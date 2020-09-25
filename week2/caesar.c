// check for one argument input representing the key
// check that input is an integer
// prompt user for plaintext input
// loop through input and shift each letter by the key
// if not a letter, just print out the character
// print newline

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool check_key(string s);

int main(int argc, string argv[])
{
    if (argc != 2 || !check_key(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // store key as a variable
        int key = atoi(argv[1]);

        // prompt user for plaintext string
        string plaintext = get_string("plaintext: ");

        printf("ciphertext: ");

        // shift all chars in plaintext by the key value
        for (int x = 0, len = strlen(plaintext); x < len; x++)
        {
            char character = plaintext[x];
            if (isalpha(character))
            {
                char start = 'A';
                if (islower(character))
                    start = 'a';
                    printf("%c", (character - start + key) % 26 + start);
            }
            else
            {
                printf("%c", character);
            }
        }
        printf("\n");

        return 0;
    }

}


bool check_key(string s)
{
    for (int i = 0, length = strlen(s); i < length; i++)
        if (!isdigit(s[i]))
        {
            return false;
        }
    return true;
}