#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <spawn.h>
#include <signal.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char **environ;

  pid_t pid;
  int done=0;
  int watchdogr=0;

  typedef struct _progs {
      int id;
      char *name;
      char *arg;
  } PROGS;

  PROGS progs[] = {
      {0, "waitprobe", ""},
      {0, "waitprobe", ""},
      {0, NULL, NULL}
  };

void term(int signum)
{
    int status;
  printf("Received SIGTERM, exiting...\n");
  watchdogr=1;
  kill(pid,SIGTERM);
  waitpid(pid, &status, 0);
  done=1;
}

void start(int n) {

  char *argv[] = {(char *) 0};
  int status;
  puts("Testing posix_spawn");
  fflush(NULL);
  status = posix_spawn(&progs[n].id, "waitprobe", NULL, NULL, argv, environ);
  if (status == 0) {
    printf("Child id: %i\n", progs[n].id);
    fflush(NULL);

  } else {
    printf("posix_spawn: %s\n", strerror(status));
  }
}


void *watchdog() {
    int status;
    int i;

    while(!watchdogr) {

        for(i=0; progs[i].name; i++) {

        while ( (progs[i].id = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("\nchild %d terminated\n", progs[i].id);
            start(i);
        }
        }

        sleep(1);
    }

}


int main(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    int i;
    for(i=0; progs[i].name; i++) {
        start(i);
    }

    pthread_t th1;

    pthread_create(&th1,NULL,watchdog,NULL);

    //pthread_join(th1,NULL);
    while (!done)
    {
        sleep(1);
    }


    return 0;
}

