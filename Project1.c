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

void sigHandler (int);

int main() {
    signal(SIGUSR1, sigHandler);
    int n;
    printf("Processes: ");
    scanf("%d", &n);

    int fd[n+1][2];
    for (int i = 0; i <= n; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    pid_t parent = getpid();
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            for (int j = 0; j <= n; j++) {
                if (j != i) close(fd[j][0]); 
                if (j != i+1) close(fd[j][1]); 
            }
            printf("Child PID is: %ld\n", (long)getpid());
            messagePacket badApple;
            if(i == n-1) kill(parent, SIGUSR1);
            while (1) {
                read(fd[i][0], &badApple, sizeof(messagePacket));
                printf("Child PID %ld has the bad apple!\n", (long)getpid());
                //sleep(1);
                if(getpid()==badApple.target){
                    printf("Target found! message is:\n %s\n",badApple.message);
                }
                else{
                    printf("Message is not for %ld!\n", (long)getpid());
                }
                write(fd[i+1][1], &badApple, sizeof(messagePacket));
            }
            exit(0);
        }
    }

    for (int i = 0; i < n; i++) {
        close(fd[i][0]); 
        close(fd[i+1][1]);
    }
    pid_t value;
    char message[250];
    messagePacket badApple;
    pause();
    getchar();
    while (1) {
        printf("Enter a message: ");
        fgets(message, sizeof(message), stdin);  
        message[strcspn(message, "\n")] = '\0';
        printf("Select a PID: ");
        scanf("%d", &value);
        getchar();
        strncpy(badApple.message, message, sizeof(badApple.message));
        badApple.target = value;
        write(fd[0][1], &badApple, sizeof(messagePacket));
        read(fd[n][0], &badApple, sizeof(messagePacket));
        printf("Back at Original Node%ld! Value: %s\n",(long)getpid(), badApple.message);
        
    }

    return 0;
}

void sigHandler(int sigNum) {
if (sigNum == SIGUSR1) {
    printf("Ready To send a message!\n");
}

else {
    printf("Received an interrupt.\n");
    printf("Time to log off bud\n");
    fflush(stdout);
    kill(0, SIGTERM);
    exit(0);
}
}
