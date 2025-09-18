#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
int main()
{
 // use these variables
pid_t pid, child, origin;
 int status;
 int n;
 
printf("Processes ");
scanf("%d", &n);
origin = getpid();
for(int i = 0; i < n; i++){
pid = fork();
 if (pid < 0) {
 fprintf(stderr, "Fork failure: %s", strerror(errno));
 exit(1);
 }
 else if(pid == 0 && i+1 == n){
    printf("I am child PID %ld\n", (long) getpid());
    child = getpid();
    exit(0);
 }
 else if (pid == 0) {
 printf("I am child PID %ld\n", (long) getpid());
 child = getpid();
 }
 else {
 waitpid(pid, &status, 0);

printf("this processes pid: %ld Child PID %ld terminated with return status %d\n", (long)
getpid(), (long) pid, status);
break;
 }
}
 return 0;
}

