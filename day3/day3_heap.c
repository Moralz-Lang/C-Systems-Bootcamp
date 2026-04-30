#include <stdio.h>
#include <stdlib.h>

void heap_demo() {
    int *p = malloc(sizeof(int));
    *p = 555;

    free(p);

    printf("Use-after-free (UB): %d\n", *p);
}

int main() {
    heap_demo();
    return 0;
}
