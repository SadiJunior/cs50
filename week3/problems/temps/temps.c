// Practice working with structs
// Practice applying sorting algorithms

#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define NUM_CITIES 10

typedef struct
{
    string city;
    int temp;
}
avg_temp;

avg_temp temps[NUM_CITIES];

void sort_cities_selection_sort(void);
void sort_cities_bubble_sort(void);

int main(void)
{
    temps[0].city = "Austin";
    temps[0].temp = 97;

    temps[1].city = "Boston";
    temps[1].temp = 82;

    temps[2].city = "Chicago";
    temps[2].temp = 85;

    temps[3].city = "Denver";
    temps[3].temp = 90;

    temps[4].city = "Las Vegas";
    temps[4].temp = 105;

    temps[5].city = "Los Angeles";
    temps[5].temp = 82;

    temps[6].city = "Miami";
    temps[6].temp = 97;

    temps[7].city = "New York";
    temps[7].temp = 85;

    temps[8].city = "Phoenix";
    temps[8].temp = 107;

    temps[9].city = "San Francisco";
    temps[9].temp = 66;

    //sort_cities_selection_sort();
    sort_cities_bubble_sort();

    printf("\nAverage July Temperatures by City\n\n");

    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}

// Sorts the cities by the Selection Sort algorithm.
void sort_cities_selection_sort(void)
{
    // Goes through the elements up to arr_size - 1.
    for (int i = 0; i < NUM_CITIES - 1; i++)
    {
        // The "seeds" for starting looking for the highest values.
        int highest_temp = temps[i].temp;
        int highest_index = i;

        for (int j = i; j < NUM_CITIES; j++)
        {
            int temp = temps[j].temp;

            // Gets the highest one from the starting point of i.
            if (temp > highest_temp)
            {
                // New highest.
                highest_temp = temp;
                highest_index = j;
            }
            else if (temp == highest_temp)
            {
                string highest_city = temps[i].city;
                string city = temps[j].city;

                // If the first string (city) is greater than the second string (highest_city).
                if (strcmp(city, highest_city) > 0)
                {
                    // New highest.
                    highest_city = city;
                    highest_index = j;
                }
            }
        }

        // Swaps the values.
        avg_temp temporary_struct = temps[i];

        temps[i] = temps[highest_index];
        temps[highest_index] = temporary_struct;
    }
}

// Sorts the cities by the Bubble Sort algorithm.
void sort_cities_bubble_sort(void)
{
    bool has_swapped = false;

    do
    {
        // Flag that indicates whether the given iteration had any item that had to be swapped or not.
        // Will be used to tell if the sorting has been completed.
        has_swapped = false;

        // Makes two counters i and j. I goes up to arr_size - 1 and j is i + 1, for always geting the next element to compare.
        for (int i = 0, j = i + 1; i < NUM_CITIES - 1; i++, j = i + 1)
        {
            avg_temp current_struct = temps[i];
            avg_temp next_struct = temps[j];

            bool should_swap = false;

            if (next_struct.temp > current_struct.temp)
            {
                should_swap = true;
            }
            else if (next_struct.temp == current_struct.temp)
            {
                if (strcmp(current_struct.city, next_struct.city) < 0)
                {
                    should_swap = true;
                }
            }

            if (should_swap)
            {
                // Swaps the values.
                avg_temp temporary_struct = temps[i];

                temps[i] = temps[j];
                temps[j] = temporary_struct;

                has_swapped = true;
            }
        }
    }
    while (has_swapped != false);
}