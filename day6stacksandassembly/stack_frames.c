#include <stdio.h>

void function_b()
{
    int b = 200;

    printf("Function B\n");
    printf("&b = %p\n", (void*)&b);
}

void function_a()
{
    int a = 100;

    printf("Function A\n");
    printf("&a = %p\n", (void*)&a);

    function_b();
}

int main()
{
    int main_var = 50;

    printf("Main Function\n");
    printf("&main_var = %p\n", (void*)&main_var);

    function_a();

    return 0;
}
