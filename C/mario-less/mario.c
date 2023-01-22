#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int blocks;
    //Check if input is a +ve Z b/w 1 and 8
    do
    {
        blocks = get_int("Height: ");
    }
    while (blocks < 1 || blocks > 8);

    // row == i
    for (int i = 1; i <= blocks; i++)
    {
        //layer is equal to blocks-row
        for (int j = 0; j < blocks - i; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}