#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int* ptr = NULL;
    int val = *ptr;

    printf("val is: %d\n", val);

    return 0;
}