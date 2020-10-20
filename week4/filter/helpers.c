#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through each pixel (double loop)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get RGB values of each pixel
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;

            // sum and average the RGB values for each pixel
            float gs_value = (red + green + blue) / 3;

            // set value of each pixel to the grayscale value
            image[i][j].rgbtRed = (int)round(gs_value);
            image[i][j].rgbtGreen = (int)round(gs_value);
            image[i][j].rgbtBlue = (int)round(gs_value);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through each pixel (double loop)
    for (int ia = 0; ia < height; ia++)
    {
        for (int ja = 0; ja < width; ja++)
        {
            // get RGB values of each pixel
            float originalRed = image[ia][ja].rgbtRed;
            float originalGreen = image[ia][ja].rgbtGreen;
            float originalBlue = image[ia][ja].rgbtBlue;

            // set value of each pixel to the grayscale value
            float s_red = .393 * originalRed + .769 * originalGreen + .189 * originalBlue;
            float s_green = .349 * originalRed + .686 * originalGreen + .168 * originalBlue;
            float s_blue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue;

            if (s_red > 255)
            {
                image[ia][ja].rgbtRed = 255;
            }
            else
            {
                image[ia][ja].rgbtRed = (int)round(s_red);
            }
            if (s_green > 255)
            {
                image[ia][ja].rgbtGreen = 255;
            }
            else
            {
                image[ia][ja].rgbtGreen = (int)round(s_green);
            }
            if (s_blue > 255)
            {
                image[ia][ja].rgbtBlue = 255;
            }
            else
            {
                image[ia][ja].rgbtBlue = (int)round(s_blue);
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through each pixel (double loop)
    for (int ib = 0; ib < height; ib++)
    {
        // loop until the halfway point
        for (int jb = 0; jb < width / 2; jb++)
        {
            // store current pixel in temp storage
            RGBTRIPLE temp = image[ib][jb];

            // swap current pixel with opposite pixel
            image[ib][jb] = image[ib][width - (jb + 1)];
            image[ib][width - (jb + 1)] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make a copy of the original image
    RGBTRIPLE copy[height][width];
    for (int ch = 0; ch < height; ch++)
    {
        for (int cw = 0; cw < width; cw++)
        {
            copy[ch][cw] = image[ch][cw];
        }
    }

    // loop through each pixel (double loop)
    for (int ic = 0; ic < height; ic++)
    {
        for (int jc = 0; jc < width; jc++)
        {
            // initialize pixel counter and blur RGB values
            float counter = 0, b_red = 0, b_green = 0, b_blue = 0;

            // loop through the surrounding 9 pixels
            for (int h = 0; h < 3; h++)
            {
                for (int w = 0; w < 3; w++)
                {
                    // check if the pixel exists i.e. not beyond the image
                    if ((ic + h - 1 >= 0 && ic + h - 1 < height) && (jc + w - 1 >= 0 && jc + w - 1 < width))
                    {
                        // sum RGB values
                        b_red += (float)copy[ic + h - 1][jc + w - 1].rgbtRed;
                        b_green += (float)copy[ic + h - 1][jc + w - 1].rgbtGreen;
                        b_blue += (float)copy[ic + h - 1][jc + w - 1].rgbtBlue;
                        counter++;
                    }
                }
            }

            // set RGB values of each pixel to the blur values (average of the above)
            image[ic][jc].rgbtRed = (int)round(b_red / counter);
            image[ic][jc].rgbtGreen = (int)round(b_green / counter);
            image[ic][jc].rgbtBlue = (int)round(b_blue / counter);
        }
    }
    return;
}
