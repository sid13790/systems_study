#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{

  int nloops = 1000000;

  int fd = open("./temp.txt", O_CREAT | O_RDWR, S_IRWXU);

  struct timeval start;
  struct timeval end;

  gettimeofday(&start, NULL);
  for (int i = 0; i < nloops; i++)
  {
    read(fd, NULL, 0);
  }
  gettimeofday(&end, NULL);

  printf("the 'read' syscall takes %f microseconds\n", (float)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / nloops);

  close(fd);

  return 0;
}