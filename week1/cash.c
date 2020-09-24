#include <stdio.h>
#include <cs50.h>
#include <math.h>

// 1. get non-negative float input
float get_positive_float(string prompt);

float get_positive_float(string prompt)
{
    float n;
    do
    {
        n = get_float("%s", prompt);
    }
    while (n <= 0.00);
    return n;
}

int main(void)
{
    float dollars = get_positive_float("Change owed:\n");

// 2. convert dollars into cents
    int cents = round(dollars * 100);

// 3. keep track of number of coins used
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    int coins = 0;

    while (cents >= quarter)
    {
        cents -= quarter;
        coins++;

    }

    while (cents >= dime)
    {
        cents -= dime;
        coins++;

    }

    while (cents >= nickel)
    {
        cents -= nickel;
        coins++;

    }

    while (cents >= penny)
    {
        cents -= penny;
        coins++;

    }

    printf("%i\n", coins);
}