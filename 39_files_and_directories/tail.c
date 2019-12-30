#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    // assert(argc == 2);
    // char* filename = argv[1];
    int opt;
    int num_lines = 10;
    char filename[256];
    char buf[1024] = { 0 };
    char cur = '\0';
    int file_size = 0;
    int line_count = 0;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                num_lines = atoi(optarg);
                break;
        }
    }

    if (optind < argc) {
        strcpy(filename, argv[optind]);
    } else {
        fprintf(stderr, "ERR: must specify a filename\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ((file_size = lseek(fd, 0, SEEK_END)) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    if (file_size == 0) {
        exit(EXIT_SUCCESS);
    }

    int read_sz;
    int offset = file_size;
    while (offset > 0) {
        if (lseek(fd, --offset, SEEK_SET) < 0) {
            perror("lseek loop");
            exit(EXIT_FAILURE);
        }

        if ((read_sz = read(fd, &cur, 1)) == 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (cur == '\n') {
            // printf("\\n");
            if ((line_count++) == num_lines) {
                break;
            }
        } else if (offset == file_size - 1) {
            // printf("%c", cur);
            num_lines--;
        }
    }

    offset = offset < 0 ? 0 : offset;
    if (cur == '\n' && file_size != 1) {
        offset++;
    }
    if (lseek(fd, offset, SEEK_SET) < 0) {
        perror("lseek final");
        exit(EXIT_FAILURE);
    }

    // printf("\nlc: %d, nl: %d, offset: %d\n===\n", line_count, num_lines, offset);
    // printf("\noffset: %d\nnumlines: %d\nlinecount: %d\n===\n", offset, num_lines, line_count);

    read(fd, buf, sizeof(buf));

    printf("%s", buf);

    close(fd);
    return 0;
}