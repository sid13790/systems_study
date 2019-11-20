#define _GNU_SOURCE

#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

int main()
{
  /*
        context switch benchmark
    */

  int nloops = 1000000;

  int p1_status = 0;
  int p2_status = 0;

  int pipe1[2];
  int pipe2[2];

  if (pipe(pipe1) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (pipe(pipe2) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);

  struct timeval start;
  struct timeval end;

  pid_t p1; 
  p1 = fork();
  pid_t p2; 
  
  if (p1 > 0) {
    p2 = fork();
  }

  // set affinity of processes in the parent
  if ((p1 > 0) && (p2 > 0))
  {
    if (sched_setaffinity(p1, sizeof(cpu_set_t), &mask) == -1)
    {
      perror("sched_setaffinity");
      exit(EXIT_FAILURE);
    }
    if (sched_setaffinity(p2, sizeof(cpu_set_t), &mask) == -1)
    {
      perror("sched_setaffinity");
      exit(EXIT_FAILURE);
    }

    close(pipe1[READ_END]);
    close(pipe1[WRITE_END]);
    close(pipe2[READ_END]);
    close(pipe2[WRITE_END]);

    waitpid(p1, &p1_status, 0);
    waitpid(p2, &p2_status, 0);
  }

  // first child
  if (p1 == 0 && p2 > 0)
  {
    printf("first child\n");
    gettimeofday(&start, NULL);
    for (int i = 0; i < nloops; i++) {
      write(pipe1[WRITE_END], NULL, 0);
      read(pipe2[READ_END], NULL, 0);
    }
    gettimeofday(&end, NULL);

    printf("the two context switches take %f microseconds\n", (float)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / nloops);
  }

  // second child
  if (p2 == 0 && p1 > 0)
  {
    printf("second child\n");
    for (int i = 0; i < nloops; i++) {
      read(pipe1[READ_END], NULL, 0);
      write(pipe2[WRITE_END], NULL, 0);
    }
  }
  return 0;
}