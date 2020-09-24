#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int count_letters(string input);
int count_words(string input);
int count_sentences(string input);
int calc_grade(int letters, int words, int sentences);

int main(void)
{
    string input = get_string("Text: ");
    int lettercount = count_letters(input);
    int wordcount = count_words(input);
    int sentencecount = count_sentences(input);
    calc_grade(lettercount, wordcount, sentencecount);
    // printf("%i letter(s)\n", lettercount);
    // printf("%i word(s)\n", wordcount);
    // printf("%i sentence(s)\n", sentencecount);
}

int count_letters(string input)
{
    int letters = 0;
    int i = 0;
    // loop until the end of the string
    while (input[i] != '\0')
    {
        if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
        {
            letters++;
        }
        i++;
    }
    return letters;
}

int count_words(string input)
{
    int words = 1;
    int i = 0;
    // word count is 0 if there is no input
    if (strlen(input) < 1)
    {
        words = 0;
    }
    else
    {
        while (input[i] != '\0')
        {
            if (input[i] == ' ')
            {
                words++;
            }
            i++;
        }
    }
    return words;
}

int count_sentences(string input)
{
    // ignore possibility of single line with no punctuation or false periods for this exercise
    int sentences = 0;
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '.' || input[i] == '!' || input[i] == '?')
        {
            sentences++;
        }
        i++;
    }
    return sentences;
}

int calc_grade(int letters, int words, int sentences)
{
    // index = 0.0588 * L - 0.296 * S - 15.8
    // L = avg num of letters per 100 words
    // S = avg num of sentences per 100 words
    float l = ((float) letters / (float) words) * 100;
    float s = ((float) sentences / (float) words) * 100;
    float grade = 0.0588 * l - 0.296 * s - 15.8;
    if (grade < 16 && grade >= 0)
    {
        printf("Grade %i\n", (int) round(grade));
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
    return 0;
}