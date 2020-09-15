#include "helpers.h"
#include <math.h>
int MAX(float val);
int xfilter[3][3] = // the Gx filter
{
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
    
};

int yfilter[3][3] = // the Gy filter 
{
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
    
};

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned int red = image[i][j].rgbtRed;
            unsigned int green = image[i][j].rgbtGreen;
            unsigned int blue = image[i][j].rgbtBlue;
            
            float avg = (red + green + blue) / 3.0; // dividing by 3 to get the average
            int average = (int)round(avg);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < floor(width / 2); j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j]; // simple swapping
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int Bred = 0;
    int Bgreen = 0;
    int Bblue = 0;
    float Bcount = 0.0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) // i and j loops to scope through the differetn pixels
        {
            Bred = 0;
            Bgreen = 0;
            Bblue = 0;
            Bcount = 0.0;
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > width - 1)
                {
                    continue;
                }
                for (int l = -1; l < 2; l++) // k and l loops to get the 8 elements surrounding the selected pixel
                {
                    if (i + l < 0 || i + l > height - 1)
                    {
                        continue;
                    }
                    Bred += image[i + l][j + k].rgbtRed; // summing the red elements from the surrounding the selected pixel
                    Bgreen += image[i + l][j + k].rgbtGreen;
                    Bblue += image[i + l][j + k].rgbtBlue;
                    Bcount++;
                }
            }
            temp[i][j].rgbtRed = round(Bred / Bcount); // averaging the the corrrespoding colors
            temp[i][j].rgbtGreen = round(Bgreen / Bcount);
            temp[i][j].rgbtBlue = round(Bblue / Bcount);
        }
    }
    // swaping pixels from temp to image file
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int Xred, Yred;
    int Xgreen, Ygreen;
    int Xblue, Yblue; // declaring the x and y components
    float Rval, Gval, Bval; // final sebel value
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Xred = 0, Yred = 0 ;
            Xgreen = 0, Ygreen = 0;
            Xblue = 0, Yblue = 0; // initializing the x and y components of colors of the pixels
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > width - 1)
                {
                    continue;
                }
                for (int l = -1; l < 2; l++)
                {
                    if (i + l < 0 || i + l > height - 1)
                    {
                        continue;
                    }
                    Xred += image[i + l][j + k].rgbtRed * xfilter[l + 1][k + 1]; // multiplying the corresponding Gx with image pixel color
                    Xgreen += image[i + l][j + k].rgbtGreen * xfilter[l + 1][k + 1];
                    Xblue += image[i + l][j + k].rgbtBlue * xfilter[l + 1][k + 1];
    
                    Yred += image[i + l][j + k].rgbtRed * yfilter[l + 1][k + 1];
                    Ygreen += image[i + l][j + k].rgbtGreen * yfilter[l + 1][k + 1];
                    Yblue += image[i + l][j + k].rgbtBlue * yfilter[l + 1][k + 1];
                }
            }
            // sebel calculation
            Rval = sqrt(pow(Xred, 2) + pow(Yred, 2));
            Gval = sqrt(pow(Xgreen, 2) + pow(Ygreen, 2));
            Bval = sqrt(pow(Xblue, 2) + pow(Yblue, 2));
    
            temp[i][j].rgbtRed = MAX(Rval);
            temp[i][j].rgbtGreen = MAX(Gval); // using the MAX function for capping
            temp[i][j].rgbtBlue = MAX(Bval);
        }
    }
    for (int i = 0; i < width; i++) // default boilerplate to repack the image file from temp
    {
        for (int j = 0; j < height; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
}

int MAX(float val) // custom capping function
{
    if (val > 255.0)
    {
        return 255;
    }
    else
    {
        return (int)round(val);
    }
    
}