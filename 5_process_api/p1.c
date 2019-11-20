#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  pid_t pid = fork();

  int x = 100;

  if (pid < 0) {
    printf("error occurred\n");
    exit(1);
  } else if (pid == 0) {
    // child
    x = 1;
    printf("x is %d\n", x);
  } else {
    // parent
    wait(NULL);
    x = 2;
    printf("x is %d\n", x);
  }

  return 0;
}
