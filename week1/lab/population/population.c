#include <cs50.h>
#include <stdio.h>
#include <string.h>

int get_size(char *message, int lowerLimit);

int main(void)
{
    const int minimumStartSize = 9;

    // Gets the data from the user.
    int startSize = get_size("Initial Llama population size: ", minimumStartSize);
    int endSize = get_size("End Llama population size: ", startSize);

    int llamaPopulation = startSize;

    int yearCount = 0;

    // Gets the total time to reach the end population.
    while (llamaPopulation < endSize)
    {
        int birthCount = llamaPopulation / 3;
        int deathCount = llamaPopulation / 4;

        llamaPopulation += birthCount - deathCount;

        yearCount++;
    }

    printf("Years: %d", yearCount);

    // Could not use my pretty message :(
    //printf("Number of years for reaching %d Llamas: %d year(s).\n", endSize, yearCount);
}

int get_size(char *message, int lowerLimit)
{
    int size = 0;

    while (size < lowerLimit)
    {
        // Not the best way to do this...
        printf("%s", message);
        size = get_int("");

        if (size < lowerLimit)
        {
            printf("Invalid value! Try again.\n");
        }
    }

    return size;
}
