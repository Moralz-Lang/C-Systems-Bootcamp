#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int pipefd[2];

    pipe(pipefd);

    pid_t pid = fork();

    if(pid == 0)
    {
        close(pipefd[1]);

        char buffer[64];

        read(pipefd[0], buffer, sizeof(buffer));

        printf("CHILD RECEIVED: %s\n", buffer);
    }
    else
    {
        close(pipefd[0]);

        char *msg = "Hello from parent";

        write(pipefd[1], msg, strlen(msg) + 1);
    }

    return 0;
}
