#include <unistd.h>
#include <stdio.h>

int main() {
    printf("page size: %d\n", getpagesize());
    return 0;
}