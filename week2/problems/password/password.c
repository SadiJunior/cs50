// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// Verifies if the given password is valid.
bool valid(string password)
{
    bool has_lower_case = false;
    bool has_upper_case = false;
    bool has_number = false;
    bool has_symbol = false;

    int n = 0;

    // Iterates through all the string chars.
    while (password[n] != '\0')
    {
        char c = password[n];

        if (islower(c))
        {
            has_lower_case = true;
        }
        else if (isupper(c))
        {
            has_upper_case = true;
        }
        else if (isdigit(c))
        {
            has_number = true;
        }
        else if (ispunct(c))
        {
            has_symbol = true;
        }

        n++;
    }

    // If satisfies all conditions.
    bool is_valid_password =
        has_lower_case &&
        has_upper_case &&
        has_number &&
        has_symbol;

    return is_valid_password;
}
