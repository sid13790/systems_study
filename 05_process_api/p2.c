#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main() {

  pid_t pid = fork();

  int fd = open("testfile.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

  // parent string
  char* parent_str = strdup("hello from parent\n");
  size_t parent_str_len = strlen(parent_str) + 1;

  // child string
  char* child_str = strdup("hello from child\n");
  size_t child_str_len = strlen(child_str) + 1;


  if (pid < 0) {
    printf("error occurred\n");
    exit(1);
  } else if (pid == 0) {
    // child
    write(fd, child_str, child_str_len);
  } else {
    // parent
    write(fd, parent_str, parent_str_len);
    wait(NULL);
  }

  free(parent_str);
  free(child_str);

  return 0;
}
