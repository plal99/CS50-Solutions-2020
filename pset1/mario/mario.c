#include <cs50.h>
#include <stdio.h>

void print(char c, int times); // beautify the program by declaring the function

int main()
{
    int num;
    do
    {
        num = get_int("Height: "); // input the height
    }
    while (num < 1 || num > 8);

    for (int i = 0; i < num; i++)
    {
        print(' ', num - i - 2);
        print('#', i);
        printf("  ");
        print('#', i);
        printf("\n");
    }
}

void print(char c, int times)    // function to print the charachters in a loop
{
    while (times >= 0)
    {
        printf("%c", c);
        times--;
    }
}
