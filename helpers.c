#include "helpers.h"
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            average = round(average / 3);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE memory = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = memory;
        }
    }
}

int valid(int i, int j, int ip, int jp, int height, int width)
{
    if (i == 0 && ip == -1)
    {
        return 0;
    }
    if (i == height - 1 && ip == 1)
    {
        return 0;
    }
    if (j == 0 && jp == -1)
    {
        return 0;
    }
    if (j == width - 1 && jp == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //buffering
    RGBTRIPLE buffer [height][width];
    for (int i  = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][j] = image[i][j];
        }
    }
    //bluring
    float r, g, b;
    int v = 0;
    for (int i = 0; i < height; i++)//for each row
    {
        for (int j = 0; j < width; j++)//every pixel in that row
        {
            r = 0, g = 0, b = 0, v = 0;//reseting rgb values
            for (int ip = -1; ip < 2; ip++) //every row above, current and bellow
            {
                for (int jp = -1; jp < 2; jp++)//every pixel in those rows
                {
                    if (valid(i, j, ip, jp, height, width))
                    {
                        r += buffer[i + ip][j + jp].rgbtRed;
                        g += buffer[i + ip][j + jp].rgbtGreen;
                        b += buffer[i + ip][j + jp].rgbtBlue;
                        v++;
                    }
                }
            }
            //saving new values
            image[i][j].rgbtRed = round(r / v);
            image[i][j].rgbtGreen = round(g / v);
            image[i][j].rgbtBlue = round(b / v);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float rx = 0, ry = 0, gx = 0, gy = 0, bx = 0, by = 0;
    RGBTRIPLE buffer [height][width];
    int Gx [3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy [3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    for (int i = 0; i < height; i++)//buffering
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i][j] = image[i][j];
        }
    }
    for (int i = 0; i < height; i++)//detecting edges
    {
        for (int j = 0; j < width; j++)
        {
            rx = 0, ry = 0, gx = 0, gy = 0, bx = 0, by = 0;
            for (int ip = -1; ip < 2; ip++)
            {
                for (int jp = -1; jp < 2; jp++)
                {
                    if (valid(i, j, ip, jp, height, width))
                    {
                        rx += buffer[i + ip][j + jp].rgbtRed * Gx[ip + 1][jp + 1];
                        ry += buffer[i + ip][j + jp].rgbtRed * Gy[ip + 1][jp + 1];
                        gx += buffer[i + ip][j + jp].rgbtGreen * Gx[ip + 1][jp + 1];
                        gy += buffer[i + ip][j + jp].rgbtGreen * Gy[ip + 1][jp + 1];
                        bx += buffer[i + ip][j + jp].rgbtBlue * Gx[ip + 1][jp + 1];
                        by += buffer[i + ip][j + jp].rgbtBlue * Gy[ip + 1][jp + 1];
                    }
                }
            }
            int r = round(sqrt((rx * rx) + (ry * ry)));
            if (r > 255)
            {
                r = 255;
            }  
            int g = round(sqrt((gx * gx) + (gy * gy)));
            if (g > 255)
            {
                g = 255;
            }
            int b = round(sqrt((bx * bx) + (by * by)));
            if (b > 255)
            {
                b = 255;
            }
            //saving results in image array
            image[i][j].rgbtRed = r;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtBlue = b;
        }
    }
}