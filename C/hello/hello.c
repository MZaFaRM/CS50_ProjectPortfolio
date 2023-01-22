#include<stdio.h>
//CS50 library
#include <cs50.h>

int main(void)
{
    //gets string
    string name = get_string("What's your name? ");
    printf("Hello, %s!\n", name);
}