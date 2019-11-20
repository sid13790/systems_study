#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

void 
sigcont_handler(int signum) {
    printf("goodbye\n");
}


int 
main() {

  pid_t parent_pid = getpid();
  pid_t child_pid = fork();

  struct sigaction act;
  act.sa_handler = sigcont_handler;

  if (child_pid < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (child_pid == 0) {
    // child
    printf("hello\n");
    kill(parent_pid, SIGCONT);
  } else {
    // parent
    sigaction(SIGCONT, &act, NULL);
    pause();
    wait(NULL);
  }

  return 0;
}
