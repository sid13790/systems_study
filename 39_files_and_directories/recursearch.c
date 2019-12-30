#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void recur_list_dir(char* dirname, int level) {
    DIR* dp = opendir(dirname);
    assert(dp != NULL);

    struct dirent* d;
    while ((d = readdir(dp)) != NULL) {
        printf("%*s", level * 4, "");
        printf("%lu %s\n", d->d_ino, d->d_name);
        if (d->d_type == 4 && strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
            char sub_dirname[1024];
            snprintf(sub_dirname, 1024, "%s/%s", dirname, d->d_name);
            recur_list_dir(sub_dirname, level + 1);
        }
    }

    closedir(dp);
}

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

    recur_list_dir(dir, 0);

    return 0;
}