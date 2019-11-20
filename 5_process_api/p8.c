#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main()
{

  int pipefd[2];

  if (pipe(pipefd) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t c1_pid = fork();
  int c1_status = 0;

  if (c1_pid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (c1_pid == 0)
  {
    // child 1

    printf("first child\n");

    if (dup2(pipefd[WRITE_END], STDOUT_FILENO) == -1)
    {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(pipefd[READ_END]);
    close(pipefd[WRITE_END]);

    char *c1_args[2];
    c1_args[0] = strdup("/bin/ls");
    c1_args[1] = NULL;

    int c1_exec_status  = execvp(c1_args[0], c1_args);
    if (c1_exec_status == -1) {
      perror("exec");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    // parent

    pid_t c2_pid = fork();
    int c2_status = 0;

    if (c2_pid < 0)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (c2_pid == 0)
    {
      // child 2

      printf("second child\n");

      if (dup2(pipefd[READ_END], STDIN_FILENO) == -1)
      {
        perror("dup2");
        exit(EXIT_FAILURE);
      }
      close(pipefd[WRITE_END]);
      close(pipefd[READ_END]);

      char *c2_args[2];
      c2_args[0] = strdup("/usr/bin/wc");
      c2_args[1] = NULL;

      int c2_exec_status  = execvp(c2_args[0], c2_args);
      if (c2_exec_status == -1) {
        perror("exec");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      // parent

      close(pipefd[WRITE_END]);
      close(pipefd[READ_END]);

      pid_t finished_c1_pid = waitpid(c1_pid, &c1_status, 0);
      printf("finished c1 process: %d\n", finished_c1_pid);

      pid_t finished_c2_pid = waitpid(c2_pid, &c2_status, 0);
      printf("finished c2 process: %d\n", finished_c2_pid);
    }

  }

  return 0;
}
