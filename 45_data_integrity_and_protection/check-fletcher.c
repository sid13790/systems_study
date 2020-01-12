#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

int main(int argc, char** argv) {

    char* filename;
    unsigned char s1 = 0;
    unsigned char s2 = 0;

    if (argc != 2) {
        fprintf(stderr, "no file given\n");
        exit(EXIT_FAILURE);
    } else {
        filename = argv[1];
    }

    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "invalid fd\n");
        exit(EXIT_FAILURE);
    }

    unsigned char c;

    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (int i = 0; i < 100; i++) {
        s1 = 0;
        s2 = 0;

        while (read(fd, &c, 1) == 1) {
            s1 = s1 + c;
            s2 = s2 + s1;
        }
        lseek(fd, 0, SEEK_SET);
    }
    
    
    gettimeofday(&end, NULL);

    float time = (float)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000;

    printf("s1: 0x%X, s2: 0x%X, took %f seconds\n", s1, s2, time);
    return 0;
}