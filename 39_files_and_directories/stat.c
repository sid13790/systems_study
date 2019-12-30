#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


int main(int argc, char* argv[]) {
    assert(argc == 2);
    char* filename = argv[1];

    struct stat info;
    stat(filename, &info);

    printf("file size: %ld\nnum blocks: %ld\nnum links: %ld\ninode num: %lu\n", info.st_size, info.st_blocks, info.st_nlink, info.st_ino);
}
