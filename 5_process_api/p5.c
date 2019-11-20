#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int 
main() {

  pid_t pid = fork();

  if (pid < 0) {
    printf("error occurred\n");
    exit(1);
  } else if (pid == 0) {
    // child
    pid_t finished_pid = wait(NULL);
    printf("finished process: %d\n", finished_pid);
  } else {
    // parent
    pid_t finished_pid = wait(NULL);
    printf("finished process: %d\n", finished_pid);
  }

  return 0;
}
