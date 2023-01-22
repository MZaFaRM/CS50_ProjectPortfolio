#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int blocks, i, j, k;

    // Verify user's input
    do
    {
        blocks = get_int("Height: ");
    }
    while (blocks < 1 || blocks > 8);

    // i is the total row
    for (int r = 1; r <= blocks; r++)
    {

        // LHS's Spaces and Hashes
        for (i = 0; i < blocks - r; i++)
        {
            printf(" ");
        }
        for (j = 0; j < r; j++)
        {
            printf("#");
        }

        // Space b/w two pyramids
        printf("  ");

        // RHS's Hashes (No Spaces)
        for (i = 0; i < r; i++)
        {
            printf("#");
        }

        // Goes to next row
        printf("\n");
    }
}