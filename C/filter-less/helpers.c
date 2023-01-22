#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][j].rgbtBlue;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Sepia Algorithm

            float Red, Green, Blue;

            Red = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            Green = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            Blue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;

            // Rounding of all values
            image[i][j].rgbtRed = round(Red);
            image[i][j].rgbtBlue = round(Blue);
            image[i][j].rgbtGreen = round(Green);

            // Cap the value at 255
            if (Red > 255)
            {
                image[i][j].rgbtRed = 255;
            }

            if (Green > 255)
            {
                image[i][j].rgbtGreen = 0xff;
            }

            if (Blue > 255)
            {
                image[i][j].rgbtGreen = 0xff;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempimage[height][width];

    //Creating the copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width - 1; j < width; j++, n--)
        {
            tempimage[i][j] = image[i][n];
        }
    }
    // Reflecting part
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempimage[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int Red = 0;
            int Green = 0;
            int Blue = 0;

            float denominator = 0.0;

            // Single Pixel's sorroundings
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    int a = i + m;
                    int b = j + n;

                    if (a < 0 || a > (height - 1) || b < 0 || b > (width - 1))
                    {
                        continue;
                    }

                    Red += image[a][b].rgbtRed;
                    Blue += image[a][b].rgbtBlue;
                    Green += image[a][b].rgbtGreen;

                    denominator++;
                }

                // Changing the value of the copy
                copy[i][j].rgbtRed = round(Red / denominator);
                copy[i][j].rgbtBlue = round(Blue / denominator);
                copy[i][j].rgbtGreen = round(Green / denominator);

            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Change the orginal value
            image[i][j] = copy[i][j];
        }
    }
    return;
}