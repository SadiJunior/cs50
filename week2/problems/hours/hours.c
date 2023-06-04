#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

float calc_hours(int hours[], int weeks, char output);

int main(void)
{
    // Gets the number of weeks from the user.
    int weeks = get_int("Number of weeks taking CS50: ");
    int hours[weeks];

    // Gets the hours from all weeks from the user.
    for (int i = 0; i < weeks; i++)
    {
        hours[i] = get_int("Week %i HW Hours: ", i);
    }


    // Gets the operation from the user.
    char output;
    do
    {
        output = toupper(get_char("Enter T for total hours, A for average hours per week: "));
    }
    while (output != 'T' && output != 'A');

    // Calculates the Total/Average hours.
    printf("%.1f hours\n", calc_hours(hours, weeks, output));
}

// Gets the Total/Average hours.
float calc_hours(int hours[], int weeks, char output)
{
    float total_hours = 0.0;

    // Gets the total hours.
    for (int i = 0; i < weeks; i++)
    {
        total_hours += hours[i];
    }

    // Does the operation defined by "output".
    switch (output)
    {
        case 'T':
            return total_hours;

        case 'A':
            return total_hours / weeks;

        default:
            return 0;
    }
}