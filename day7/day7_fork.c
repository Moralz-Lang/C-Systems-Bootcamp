#include <stdio.h>
#include <unistd.h>

int main()
{
    int x = 100;

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if(pid == 0)
    {
        x += 50;

        printf("CHILD: x = %d\n", x);
    }
    else
    {
        x -= 50;

        printf("PARENT: x = %d\n", x);
    }

    return 0;
}
