#include <stdio.h>
#include <stdlib.h>

void buffer_overflow_demo() {
    int arr[3] = {1,2,3};

    for(int i=0;i<5;i++) {
        arr[i] = i;
    }
}

void use_after_free_demo() {
    int *p = malloc(sizeof(int));
    *p = 42;

    free(p);

    printf("Use-after-free: %d\n", *p);
}

void double_free_demo() {
    int *p = malloc(sizeof(int));

    free(p);
    free(p);
}

void memory_leak_demo() {
    int *p = malloc(sizeof(int));
    *p = 123;
}

int main() {
    buffer_overflow_demo();
    use_after_free_demo();
    double_free_demo();
    memory_leak_demo();
    return 0;
}
