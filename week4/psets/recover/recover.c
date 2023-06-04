#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover file\n");
        return 1;
    }

    // Open input file for reading
    FILE *data_file = fopen(argv[1], "r");

    if (data_file == NULL)
    {
        fclose(data_file);
        printf("Could not open file.\n");
        return 1;
    }

    string filename = malloc(sizeof(char) * 8);

    int jpg_count = 0;

    bool is_writing = false;

    FILE *jpg_file = NULL;

    uint8_t buffer[BLOCK_SIZE];

    while (fread(&buffer, BLOCK_SIZE, 1, data_file))
    {
        // Verifies if the first four bytes are the prefix of JPG files
        bool is_jpg_prefix =
            buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0;

        // If it is a JPG file, and it is not currently writing one
        // In other words, if it is the start of a JPG file
        if (is_jpg_prefix)
        {
            // If already opened a file
            if (jpg_count != 0)
            {
                fclose(jpg_file);
            }

            sprintf(filename, "%03i.jpg", jpg_count);
            jpg_count++;

            jpg_file = fopen(filename, "w");
            fwrite(&buffer, BLOCK_SIZE, 1, jpg_file);
        }
        // If one JPG file is found, all the other are adjacent
        else if (jpg_count > 0)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, jpg_file);
        }
    }

    // Free the memory
    fclose(data_file);
    fclose(jpg_file);
    free(filename);
}