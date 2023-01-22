#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number = 0, temp, temp1, temp2;
    int rem = 0, i, product = 0, ps = 0, sum = 0;
    int result = 0, digits = 0, Total_sum = 0;


    number = get_long("Number: ");

    temp = temp1 = temp2 = number;
    for (i = 0; temp != 0; i++)
    {
        // removes last digit
        temp = temp / 10;
        // stores second last digit
        rem = temp % 10;
        // rem * 2
        product = 2 * rem;
        // removes the second last digit
        temp = temp / 10;
        // if the product is two digit
        if (product >= 10)
        {
            // adds 1 and the other digit to ps
            // 1 because 18 is the highest possible product
            rem = product % 10;
            // ps is the product's sum
            ps = rem + ps + 1;
        }
        else
        {
            // product is one digit
            ps = ps + product;
        }
        // repeats the loop
    }
    for (i = 0; temp1 != 0; i++)
    {
        // adding the other digits
        rem = temp1 % 10;
        temp1 = temp1 / 100;
        sum = rem + sum;
    }
    Total_sum = sum + ps;
    result = Total_sum % 10;
    while (temp2 > 100)
    {
        temp2 = temp2 / 10;
    }
    if (result == 0)
    {
        while (number != 0)
        {
            digits++;
            number = number / 10;
        }
        if (temp2 == 34 || temp2 == 37)
        {
            if (digits == 15)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (temp2 / 10 == 4)
        {
            if (digits == 16 || digits == 13)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (temp2 <= 55 && temp2 >= 51)
        {
            if (digits == 16)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}