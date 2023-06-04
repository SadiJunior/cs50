#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

#include "wav.h"

// Number of bytes in a .WAV File.
#define WAV_HEADER_SIZE 44

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *input = fopen(argv[1], "r");

    if (input == NULL)
    {
        fclose(input);
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    bool is_wav_file = check_format(header) == 0;

    if (!is_wav_file)
    {
        fclose(input);
        printf("Not a .WAV File.\n");
        return 1;
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");

    if (output == NULL)
    {
        fclose(input);
        fclose(output);
        printf("Could not create file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    // Write reversed audio to file
    uint8_t buffer[block_size];

    // Points to the end of the input file
    fseek(input, 0, SEEK_END);

    // Iterates through all data in reverse order, until gets to the Header (44 bytes)
    while (ftell(input) > sizeof(WAVHEADER))
    {
        // Points to the "next" block in reverse order
        fseek(input, -block_size, SEEK_CUR);

        // Reads the block in the buffer
        fread(&buffer, block_size, 1, input);

        // As it is now pointing to the end, must point back to the last position.
        fseek(input, -block_size, SEEK_CUR);

        // Writes the block to the output file
        fwrite(&buffer, block_size, 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}

// Verifies if the given header is from a .WAV file
int check_format(WAVHEADER header)
{
    const int FORMAT_SIZE = 4;
    const char WAV_FORMAT[FORMAT_SIZE] = { 'W', 'A', 'V', 'E' };

    for (int i = 0; i < FORMAT_SIZE; i++)
    {
        // If one character does not match the WAV_FORMAT, then it is not a .WAV file
        if (header.format[i] != WAV_FORMAT[i])
        {
            return 1;
        }
    }

    // All characters matched
    return 0;
}

// Gets the data block size
int get_block_size(WAVHEADER header)
{
    uint16_t num_of_channels = header.numChannels;
    uint8_t bytes_per_sample = header.bitsPerSample / 8;

    return num_of_channels * bytes_per_sample;
}