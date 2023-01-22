#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating the sum of each colour and finding the average
            copy[i][j]  = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            copy[i][j] = round(copy[i][j] / 3.0);
        }
    }

    // Assigning the average to every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = copy[i][j];
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy for the orginal image
    RGBTRIPLE copy[height][width];

    // Value of the original image are stored in the horizonatlly opposite side of the vertical image
    // Thus, reflecting the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, m = width - 1; j < width; j++, m--)
        {
            copy[i][m]  = image[i][j];
        }
    }

    // Flipped values are given back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j]  = copy[i][j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of the original image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Stores the sum of each colour
            int red = 0, green = 0, blue = 0;

            // Finds the number of usable pixels around a value
            float denominator = 0.0;

            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    int a = i + m;
                    int b = j + n;

                    // Checks whether the values are out of bond
                    if (a < 0 || a > (height - 1) || b < 0 || b > (width - 1))
                    {
                        continue;
                    }

                    // Calculates the sum of colours
                    red += image[a][b].rgbtRed;
                    green += image[a][b].rgbtGreen;
                    blue += image[a][b].rgbtBlue;

                    // Increments the usable pixels when an addition takes place
                    denominator++;

                }
            }

            // Adds the values to the copy of the image
            copy[i][j].rgbtRed = round(red / denominator);
            copy[i][j].rgbtGreen = round(green / denominator);
            copy[i][j].rgbtBlue = round(blue / denominator);
        }
    }

    // Replaces the orignal image with changed values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Gx and Gy Kernels
    int Gy[3][3], Gx[3][3];

    // Copy of the orginal image
    RGBTRIPLE copy[height][width];

    // Creates the kernel
    for (int i = 0; i < 3; i++)
    {
        Gx[0][i] = i - 1;
        Gx[1][i] = 2 * (i - 1);
        Gx[2][i] = i - 1;
    }
    for (int i = 0; i < 3; i++)
    {
        Gy[i][0] = i - 1;
        Gy[i][1] = 2 * (i - 1);
        Gy[i][2] = i - 1;
    }

    // Loops for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redx = 0, greenx = 0, bluex = 0;
            int redy = 0, greeny = 0, bluey = 0;

            // For the sorroundings of each pixel
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    int a = i + m;
                    int b = j + n;

                    // To nullify values beyond the edges
                    if (a < 0 || a > (height - 1) || b < 0 || b > (width - 1))
                    {
                        redx += (Gx[a + 1][b + 1] * 0);
                        greenx += (Gx[a + 1][b + 1] * 0);
                        bluex += (Gx[a + 1][b + 1] * 0);

                        redy += (Gy[a + 1][b + 1] * 0);
                        greeny += (Gy[a + 1][b + 1] * 0);
                        bluey += (Gy[a + 1][b + 1] * 0);
                    }

                    else
                    {
                        // Each colour in a pixel is multiplied by kernel and added together
                        redx += (Gx[m + 1][n + 1] * image[a][b].rgbtRed);
                        greenx += (Gx[m + 1][n + 1] * image[a][b].rgbtGreen);
                        bluex += (Gx[m + 1][n + 1] * image[a][b].rgbtBlue);

                        redy += (Gy[m + 1][n + 1] * image[a][b].rgbtRed);
                        greeny += (Gy[m + 1][n + 1] * image[a][b].rgbtGreen);
                        bluey += (Gy[m + 1][n + 1] * image[a][b].rgbtBlue);
                    }
                }
            }

            // Sobel Operator
            int RMS_red = round(sqrt(pow(redx, 2) + pow(redy, 2)));
            int RMS_green = round(sqrt(pow(greenx, 2) + pow(greeny, 2)));
            int RMS_blue = round(sqrt(pow(bluex, 2) + pow(bluey, 2)));

            // Capping the values at 225
            if (RMS_red > 255)
            {
                RMS_red = 255;
            }

            if (RMS_green > 255)
            {
                RMS_green = 255;
            }

            if (RMS_blue > 255)
            {
                RMS_blue = 255;
            }

            // Copying the values to an duplicate of image
            copy[i][j].rgbtRed = RMS_red;
            copy[i][j].rgbtBlue = RMS_blue;
            copy[i][j].rgbtGreen = RMS_green;
        }
    }

    // Replaces the orignal image with changed values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}
