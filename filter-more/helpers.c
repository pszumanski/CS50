#include "helpers.h"
#include <math.h>
#include <stdlib.h>

#define b rgbtBlue
#define g rgbtGreen
#define r rgbtRed

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].b + image[i][j].g + image[i][j].r) / 3.0);
            image[i][j].b = avg;
            image[i][j].g = avg;
            image[i][j].r = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            buffer.b = image[i][j].b;
            buffer.g = image[i][j].g;
            buffer.r = image[i][j].r;
            image[i][j].b = image[i][width - j - 1].b;
            image[i][j].g = image[i][width - j - 1].g;
            image[i][j].r = image[i][width - j - 1].r;
            image[i][width - j - 1].b = buffer.b;
            image[i][width - j - 1].g = buffer.g;
            image[i][width - j - 1].r = buffer.r;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Saves blur for each pixel, [0] - blue [1] - green, [2] - red, [3] - counter
    int buffer[height][width][4];
    // Goes over each colum
    for (int i = 0; i < height; i++)
    {
        // Goes over each row
        for (int j = 0; j < width; j++)
        {
            // Goes over each column in 3x3 grid
            for (int k = 0; k < 3; k++)
            {
                // Goes over each row in 3x3 grid
                for (int m = 0; m < 3; m++)
                {
                    // If pixel is out of image, then skip it
                    if (i + k - 1 < 0 || i + k - 1 >= height || j + m - 1 < 0 || j + m - 1 >= width)
                    {
                        continue;
                    }
                    // Calculate blur for each pixel
                    buffer[i][j][0] += image[i + k - 1][j + m - 1].b;
                    buffer[i][j][1] += image[i + k - 1][j + m - 1].g;
                    buffer[i][j][2] += image[i + k - 1][j + m - 1].r;
                    buffer[i][j][3]++;

                }
            }
        }
    }
    // Finalizes picture
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][j][0] = round(buffer[i][j][0] / (float) buffer[i][j][3]);
            buffer[i][j][1] = round(buffer[i][j][1] / (float) buffer[i][j][3]);
            buffer[i][j][2] = round(buffer[i][j][2] / (float) buffer[i][j][3]);
            image[i][j].b = buffer[i][j][0];
            image[i][j].g = buffer[i][j][1];
            image[i][j].r = buffer[i][j][2];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Matrixes for multiplication
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Saves sobel factor, [0] - blue [1] - green, [2] - red
    int sobelX[height][width][3];
    int sobelY[height][width][3];
    // Goes over each colum
    for (int i = 0; i < height; i++)
    {
        // Goes over each row
        for (int j = 0; j < width; j++)
        {
            // Goes over each column in 3x3 grid
            for (int k = 0; k < 3; k++)
            {
                // Goes over each row in 3x3 grid
                for (int m = 0; m < 3; m++)
                {
                    // If pixel is out of image, then skip it
                    if (i + k - 1 < 0 || i + k - 1 >= height || j + m - 1 < 0 || j + m - 1 >= width)
                    {
                        continue;
                    }
                    // Calculate sobel factor for each pixel
                    sobelX[i][j][0] += image[i + k - 1][j + m - 1].b * Gx[k][m];
                    sobelX[i][j][1] += image[i + k - 1][j + m - 1].g * Gx[k][m];
                    sobelX[i][j][2] += image[i + k - 1][j + m - 1].r * Gx[k][m];
                    sobelY[i][j][0] += image[i + k - 1][j + m - 1].b * Gy[k][m];
                    sobelY[i][j][1] += image[i + k - 1][j + m - 1].g * Gy[k][m];
                    sobelY[i][j][2] += image[i + k - 1][j + m - 1].r * Gy[k][m];
                }
            }
        }
    }

    // Finalize the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].b = fmin(round(sqrt((double) pow(sobelX[i][j][0], 2) + pow(sobelY[i][j][0], 2))), 255);
            image[i][j].g = fmin(round(sqrt((double) pow(sobelX[i][j][1], 2) + pow(sobelY[i][j][1], 2))), 255);
            image[i][j].r = fmin(round(sqrt((double) pow(sobelX[i][j][2], 2) + pow(sobelY[i][j][2], 2))), 255);
        }
    }
    return;
}
