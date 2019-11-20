#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int* data = (int *) malloc(100 * sizeof(int));

    free(data);

    printf("some val: %d\n", *(data+25));

    return 0;
}