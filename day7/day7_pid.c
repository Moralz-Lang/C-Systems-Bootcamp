#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if(pid == 0)
    {
        printf("CHILD PID: %d\n", getpid());
        printf("CHILD PPID: %d\n", getppid());
    }
    else
    {
        printf("PARENT PID: %d\n", getpid());
    }

    return 0;
}
