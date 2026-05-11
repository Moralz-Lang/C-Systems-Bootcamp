#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig)
{
    printf("\nCaught signal: %d\n", sig);
}

int main()
{
    signal(SIGINT, handle_sigint);

    while(1)
    {
        printf("Running...\n");

        sleep(1);
    }

    return 0;
}
