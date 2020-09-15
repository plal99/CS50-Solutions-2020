#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("What is your name?\n"); // inputs the name
    
    printf("hello, %s\n", name); // prints the name we input
}