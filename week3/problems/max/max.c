// Practice writing a function to find a max value

#include <cs50.h>
#include <stdio.h>

int max(int array[], int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Number of elements: ");
    }
    while (n < 1);

    int arr[n];

    for (int i = 0; i < n; i++)
    {
        arr[i] = get_int("Element %i: ", i);
    }

    printf("The max value is %i.\n", max(arr, n));
}

// Returns the max value of the given array.
int max(int array[], int n)
{
    // Starts the max_value by giving a "seed" value.
    int max_value = array[0];

    // Can start at 1, as the seed is the first value of the array.
    for (int i = 1; i < n; i++)
    {
        int value = array[i];

        if (value > max_value)
        {
            max_value = value;
        }
    }

    return max_value;
}
