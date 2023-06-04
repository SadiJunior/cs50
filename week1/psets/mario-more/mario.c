#include <cs50.h>
#include <stdio.h>

void print_tower(int height);

int main(void)
{
    const int MAX_HEIGHT = 8;

    int height = 0;

    while (height <= 0 || height > 8)
    {
        height = get_int("Type the Tower Height: ");
    }

    print_tower(height);
}

// Prints the tower with given height
void print_tower(int height)
{
    char left_tower[height];
    char right_tower[height];

    // Fills both sides of the tower with spaces
    for (int i = 0; i < height; i++)
    {
        left_tower[i] = ' ';
        //right_tower[i] = ' ';
    }

    // Adds the '#' and prints the tower
    for (int i = 0; i < height; i++)
    {
        right_tower[i] = '#';
        left_tower[height - 1 - i] = '#';

        printf("%s  %s\n", left_tower, right_tower);
    }
}