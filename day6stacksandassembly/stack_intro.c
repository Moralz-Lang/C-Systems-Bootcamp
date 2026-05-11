#include <stdio.h>

void test_function()
{
    int x = 10;
    int y = 20;

    printf("x = %d\n", x);
    printf("y = %d\n", y);

    printf("&x = %p\n", (void*)&x);
    printf("&y = %p\n", (void*)&y);
}

int main()
{
    test_function();

    return 0;
}
