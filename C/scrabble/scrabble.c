#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("It's a tie!\n");
    }
}

int compute_score(string word)
{
    int points = 0;
    for (int i = strlen(word) - 1; i >= 0; i--)
    {
        //Converts all characters to lower case
        char letter = tolower(word[i]);

        //Filters punctuation
        if ('a' <= letter && letter <= 'z')
        {

            //Converts letter to ascii number
            int n = (int)(letter);

            //a is worth 97
            n = n - 97;

            //Takes the nth value and add to point
            points = points + POINTS[n];
        }
    }
    return points;
}
