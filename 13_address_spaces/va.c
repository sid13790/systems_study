#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int* heap = malloc(100e6);
    printf("location of code : %p\n", main);
    printf("location of heap : %p\n", heap);
    int x = 3;
    printf("location of stack: %p\n", &x);
    free(heap);
    return x;
}