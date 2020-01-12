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
    unsigned char checksum = 0;

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
        c = 0;
        while (read(fd, &c, 1) == 1) {
            checksum = checksum ^ c;
        }

        lseek(fd, 0, SEEK_SET);
    }

    gettimeofday(&end, NULL);

    float time = (float)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000000;

    printf("checksum: 0x%X, took %f seconds\n", checksum, time);
    return 0;
}