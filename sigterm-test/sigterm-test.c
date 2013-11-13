#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int done = 0;

void term(int signum)
{
    printf("Received SIGTERM, exiting...\n");
    done = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    int loop = 0;
    while (!done)
    {
        sleep(3);
        printf("Finished loop run %d.\n", loop++);
    }

    printf("done.\n");
    return 0;
}
