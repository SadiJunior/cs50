#include "helpers.h"
#include <math.h>

// Convert image to grayscale
// Gray tones are made of the same RED, GREEN and BLUE values.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];

            // Gets the average value of the RGB colors.
            int grey_tone = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);

            // Sets the average values to all RGB colors.
            image[i][j].rgbtRed = grey_tone;
            image[i][j].rgbtGreen = grey_tone;
            image[i][j].rgbtBlue = grey_tone;
        }
    }
}

// Reflect image horizontally
// Must swap the values from the rows.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Gets the indexes of the beginning and end of the row
        // Iterates simultaniously through them, moving towards the middle
        // And goes swapping them in the process.
        for (int b = 0, e = width - 1; b < width / 2; b++, e--)
        {
            RGBTRIPLE temp = image[i][b];

            image[i][b] = image[i][e];
            image[i][e] = temp;
        }
    }
}

// Blur image
// Gets the average of all colors in the 8 pixels surrounding one, and itself (9 pixels in total)
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copies the original image
    // This is needed, as we're going to modify and verify pixels simultaniously
    RGBTRIPLE original_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Gets the upper and lower row
            int upper_row = (i - 1 < 0) ? i : i - 1;
            int lower_row = (i + 1 >= height) ? height - 1 : i + 1;

            // Gets the left and right columns
            int left_column = (j - 1 < 0) ? j : j - 1;
            int right_column = (j + 1 >= width) ? width - 1 : j + 1;

            // Starts the variables that will store the sum of the pixels
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;

            // Gets the number of pixels that were accounted for getting the average
            int pixel_count = 0;

            // Goes through all pixels around the given pixel
            for (int k = upper_row; k <= lower_row; k++)
            {
                for (int l = left_column; l <= right_column; l++)
                {
                    pixel_count += 1;

                    RGBTRIPLE pixel = original_image[k][l];

                    total_red += pixel.rgbtRed;
                    total_green += pixel.rgbtGreen;
                    total_blue += pixel.rgbtBlue;
                }
            }

            // Gets the average RGB values
            int average_red = round((float)total_red / pixel_count);
            int average_green = round((float)total_green / pixel_count);
            int average_blue = round((float)total_blue / pixel_count);

            // Blurs the given pixel
            image[i][j].rgbtRed = average_red;
            image[i][j].rgbtGreen = average_green;
            image[i][j].rgbtBlue = average_blue;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //grayscale(height, width, image);

    // Copies the original image
    // This is needed, as we're going to modify and verify pixels simultaniously
    RGBTRIPLE original_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_image[i][j] = image[i][j];
        }
    }

    // Defines the kernels
    const int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1},
    };

    const int gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Gets the limits of the surroundings of the given pixel
            int upper_row = i - 1;
            int lower_row = i + 1;

            int left_column = j - 1;
            int right_column = j + 1;

            // Gets the limits of the convolutional matrix
            int upper_g = 0;
            int lower_g = 2;

            int left_g = 0;
            int right_g = 2;

            // Verifies and gets valid limits
            if (upper_row < 0)
            {
                upper_row = i;
                upper_g = 1;
            }
            else if (lower_row >= height)
            {
                lower_row = height - 1;
                lower_g = 1;
            }

            if (left_column < 0)
            {
                left_column = j;
                left_g = 1;
            }
            else if (right_column >= width)
            {
                right_column = width - 1;
                right_g = 1;
            }

            // Starts the variables from the pixels of x and y axis
            int red_x = 0;
            int green_x = 0;
            int blue_x = 0;

            int red_y = 0;
            int green_y = 0;
            int blue_y = 0;

            // Goes through the convolutional matrix and the surroundings of the given pixel and
            // gets the total values of RGB colours for calculating the Gx and Gy's values
            // Sorry, Style50 says that these for loops should be in one line, otherwise I would split it in three lines.
            for (int g_row = upper_g, image_row = upper_row; (g_row <= lower_g && image_row <= lower_row); g_row++, image_row++)
            {
                for (int g_column = left_g, image_column = left_column; (g_column <= right_g
                        && image_column <= right_column); g_column++, image_column++)
                {
                    int gx_factor = gx[g_row][g_column];
                    int gy_factor = gy[g_row][g_column];

                    RGBTRIPLE pixel = original_image[image_row][image_column];

                    red_x += gx_factor * pixel.rgbtRed;
                    green_x += gx_factor * pixel.rgbtGreen;
                    blue_x += gx_factor * pixel.rgbtBlue;

                    red_y += gy_factor * pixel.rgbtRed;
                    green_y += gy_factor * pixel.rgbtGreen;
                    blue_y += gy_factor * pixel.rgbtBlue;
                }
            }

            // Gets the value of all the channels using the sobel algorithm
            int sobel_diff_red = round(sqrt(pow(red_x, 2) + pow(red_y, 2)));
            int sobel_diff_green = round(sqrt(pow(green_x, 2) + pow(green_y, 2)));
            int sobel_diff_blue = round(sqrt(pow(blue_x, 2) + pow(blue_y, 2)));

            // Sets the values to the pixel
            // It can't be bigger than 255
            image[i][j].rgbtRed = sobel_diff_red > 255 ? 255 : sobel_diff_red;

            image[i][j].rgbtGreen = sobel_diff_green > 255 ? 255 : sobel_diff_green;

            image[i][j].rgbtBlue = sobel_diff_blue > 255 ? 255 : sobel_diff_blue;
        }
    }
}