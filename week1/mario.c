#include <stdio.h>
#include <cs50.h>

int get_positive_int(string prompt);

int main(void)
{
    int input = get_positive_int("Height?\n");

    // where r = rows e.g. 4
    for (int r = 0; r < input ; r++)
    {
        // where c = columns e.g. 4 = 4x4 square
        for (int c = 0; c < input ; c++)
        {
            // if the sum of row and column is greater than or equal to the input less one
            if (r + c >= input - 1)
            {
                // this creates the diagonal slope across the square
                printf("#");
            }
            else
            // if the sum of row and column is less than input less 1
            {
                // push #s to the right
                printf(" ");
            }
        }
    printf("\n");
    }
}

int get_positive_int(string prompt)
{
    int n;
    do
    {
        n = get_int("%s", prompt);
    }
    while (n < 1 || n > 8);
    return n;
}