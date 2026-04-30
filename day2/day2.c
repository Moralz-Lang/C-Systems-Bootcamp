# 🧪 DAY 1

## 📄 day1/day1.c

```c
#include <stdio.h>
#include <stdlib.h>

void inspect_memory() {
    int *heap_memory = malloc(sizeof(int));
    *heap_memory = 555;

    printf("Allocated: %d\n", *heap_memory);

    free(heap_memory);

    printf("After free (UB): %d\n", *heap_memory);
}

int main() {
    inspect_memory();
    return 0;
}
