#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    printf("PID: %d\n", getpid());

    while(1)
    {
        sleep(1);
    }

    return 0;
}
