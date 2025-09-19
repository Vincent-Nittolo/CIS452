#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

typedef struct {
    char message[250];
    pid_t target;
} messagePacket;

int main() {
    int n;
    printf("Processes: ");
    scanf("%d", &n);

    int fd[n+1][2]; // n children + 1 return pipe to parent
    for (int i = 0; i <= n; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // Child process
            for (int j = 0; j <= n; j++) {
                if (j != i) close(fd[j][0]); 
                if (j != i+1) close(fd[j][1]); 
            }
            printf("Child PID is: %ld\n", (long)getpid());
            messagePacket badApple;
            while (1) {
                read(fd[i][0], &badApple, sizeof(messagePacket));
                printf("Child PID %ld received: %s\n", (long)getpid(), badApple.message);
                sleep(1);
                if(getpid()==badApple.target){
                    printf("Target found! message is %s\n",badApple.message);
                }
                else{
                    printf("Target not found yet\n");
                }
                write(fd[i+1][1], &badApple, sizeof(messagePacket));
            }
            exit(0);
        }
    }

    // Parent process
    for (int i = 0; i < n; i++) {
        close(fd[i][0]); 
        close(fd[i+1][1]);
    }

    pid_t value;
    char message[250];
    messagePacket badApple;
    while (1) {
        printf("Enter a message to send (Ctrl+C to quit): ");
        scanf("%s", message);
        printf("Enter a target pid: ");
        scanf("%d", &value);
        strncpy(badApple.message, message, sizeof(badApple.message));
        badApple.target = value;
        write(fd[0][1], &badApple, sizeof(messagePacket));
        read(fd[n][0], &badApple, sizeof(messagePacket));
        printf("Back at Original Node%ld! Value: %s\n",(long)getpid(), badApple.message);
    }

    return 0;
}
