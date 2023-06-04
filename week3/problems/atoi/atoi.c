#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert_recursive(string input);
int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert_recursive(input));
}

// Converts recursively, by converting the last char of the given string in a lot of different functions. (I don't like this at all...)
int convert_recursive(string input)
{
    if (input[0] == '\0')
    {
        return 0;
    }

    int last_char_index = strlen(input) - 1;

    // Converts the last char to an integer.
    int number = input[last_char_index] - '0';

    // Removes the last char from the input string.
    input[last_char_index] = '\0';

    // Calls the function recursively to convert the last number of the given string (I don't like this at all).
    number += convert_recursive(input) * 10;

    return number;
}

// Converts the given string to an int. (Only for positive numbers hehe)
int convert(string input)
{
    int number = 0;

    int n = 1;

    for (int i = strlen(input) - 1; i >= 0; i--)
    {
        // Subtracts the '0' char from ASCII to get the correspondent binary of the decimal value.
        int digit = input[i] - '0';

        // Gets the total number.
        number += digit * n;
        n *= 10;
    }

    return number;
}