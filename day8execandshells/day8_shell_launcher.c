#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if(pid == 0)
    {
        execlp("ls", "ls", "-l", NULL);

        perror("exec failed");
    }
    else
    {
        wait(NULL);

        printf("Child finished\n");
    }

    return 0;
}
