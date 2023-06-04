#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets the name of the user.
    string name = get_string("What is your name? ");

    printf("Hello, %s!\n", name);
}