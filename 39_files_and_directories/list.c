#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


int main(int argc, char** argv) {
    int opt;
    char dir[256];
    while ((opt = getopt(argc, argv, "l:")) != -1) {
        switch (opt) {
            case 'l':
                strcpy(dir, optarg);
                break;
        }
    }

    // no argument passed in
    if (strlen(dir) == 0) {
        if (getcwd(dir, sizeof(dir)) != NULL) {
            printf("%s\n", dir);
        } else {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
    }

    DIR* dp = opendir(dir);
    assert(dp != NULL);

    struct dirent* d;
    while ((d = readdir(dp)) != NULL) {
        printf("%lu %s\n", d->d_ino, d->d_name);
    }
    closedir(dp);
    return 0;
}