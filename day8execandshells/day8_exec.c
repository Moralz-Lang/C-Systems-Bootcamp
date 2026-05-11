#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Before exec\n");

    execlp("ls", "ls", "-l", NULL);

    printf("This never runs\n");

    return 0;
}
