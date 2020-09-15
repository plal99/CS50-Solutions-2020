#include <stdio.h>                                                  // importing different header files 
#include <cs50.h>
#include <string.h>
#include <math.h>

int count_letters(string text, int length);
int count_words(string text, int length);                               // funciton declarations
int count_sentences(string text, int length);

int main(void)
{

    string text = get_string("Text: ");
    int n = strlen(text);
    // printf("%i letters\n", count_letters(text,n));
    // printf("%i words\n",count_words(text,n));
    // printf("%i sentences\n",count_sentences(text,n));
    float l = (float)count_letters(text, n) * 100 / (float)count_words(text, n);
    float s = (float)count_sentences(text, n) * 100 / (float)count_words(text, n);
    int index = (int)round(0.0588 * l - 0.296 * s - 15.8);
    if (index >= 16)                                                // condition checking fot printing the grades
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_sentences(string text, int length)                            // count number of sentences
{
    int sentences = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

int count_words(string text, int length)                                // count number of words
{
    int words = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    words += 1;
    return words;
}

int count_letters(string text, int length)                              // count number of letters
{
    int letters = 0;
    for (int i = 0; i < length; i++)
    {
        if (65 <= (int)text[i] && (int)text[i] <= 90)
        {
            letters++;
        }
        if (97 <= (int)text[i] && (int)text[i] <= 122)
        {
            letters++;
        }
    }
    return letters;
}