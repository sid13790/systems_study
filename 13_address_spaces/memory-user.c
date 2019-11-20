#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

    pid_t pid = getpid();
    printf("current pid: %d\n", pid);
    printf("size of int: %ld\n", sizeof(int));

    int num_mb;

    if (argc == 2) {
        num_mb = atoi(argv[1]);
    } else {
        num_mb = 1;
    }

    int arr_size = num_mb * 1048576 / sizeof(int);

    int arr[arr_size];


    int counter = 0;
    int storage = 0;

    while (true) {
        storage = arr[counter];
        counter = (counter + 1) % arr_size;
    }

    printf("storage: %d\n", storage);

    return 0;
}