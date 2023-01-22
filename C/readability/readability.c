#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main()
{
    // Implement your program in a file called readability.c in a directory called readability.
    //prompt the user for a string of text using get_string.
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text.
    int i_letters = count_letters(text);
    int i_words = count_words(text);
    int i_sentences = count_sentences(text);

    // Converts int to float
    float letters = (float)(i_letters);
    float words = (float)(i_words);
    float sentences = (float)(i_sentences);

    // Coleman-Liau formula
    float L = letters / words * 100;
    float S = sentences / words * 100;

    // Rounded to the nearest integer
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // If the resulting index number is 16 or higher, program outputs
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    // If the index number is less than 1, program outputs
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    // Program prints "Grade X"
    else
    {
        printf("Grade %d\n", index);
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        // Converts the character to lower case
        text[i] = tolower(text[i]);

        if (text[i] >= 'a' && text[i] <= 'z')
        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)
{
    // Count the number of words in the text.
    int words = 1;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    // Count the number of sentences in the text.
    int sentences = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}