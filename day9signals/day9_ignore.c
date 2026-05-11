#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    signal(SIGINT, SIG_IGN);

    while(1)
    {
        printf("Cannot CTRL+C me\n");

        sleep(1);
    }

    return 0;
}
