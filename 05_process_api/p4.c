// execvpe
#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int 
main() {

  pid_t pid = fork();

  if (pid < 0) {
    printf("error occurred\n");
    exit(1);
  } else if (pid == 0) {
    // child
    char* myargs[2];
    myargs[0] = strdup("/bin/ls");
    myargs[1] = NULL;

    /*
      execvp
    */
    // execvp(myargs[0], myargs);

    /*
      execvpe
    */
    // char *path = getenv("PATH");
    // char  pathenv[strlen(path) + sizeof("PATH=")];
    // sprintf(pathenv, "PATH=%s", path);
    // char* envp[] = {pathenv, NULL};
    // execvpe(myargs[0], myargs, envp);

    /*
      execv
    */
    // execv(myargs[0], myargs);

    /*
      execl
    */
    // execl(myargs[0], myargs[0], NULL);

    /*
      execlp
    */
    // execlp(myargs[0], myargs[0], NULL);

    /*
      execle
    */
    char *path = getenv("PATH");
    char  pathenv[strlen(path) + sizeof("PATH=")];
    sprintf(pathenv, "PATH=%s", path);
    char* envp[] = {pathenv, NULL};
    execle(myargs[0], myargs[0], NULL, envp);



  } else {
    // parent
    wait(NULL);
  }

  return 0;
}
