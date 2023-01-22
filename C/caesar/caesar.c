#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");                        // Error if argument count is not equal to 2
        return 1;
    }
    string value = argv[1];

    int m = 1;                                                  // Multiplier
    int k = 0;
    for (int i = 0; value[i] != '\0'; i++, m = m * 10)
    {
        if ((int)(value[i]) >= 48 && (int)(value[i]) <= 57)
        {
            k = (int)(((k * m) + value[i]) - 48);               // Key + 48 (because string) = Key * Multiplier + value
        }
        else
        {
            printf("Usage: ./caesar key\n");                    // Error if non-numeric values
            return 1;
        }
    }
    if (k >= 0)
    {
        string message = get_string("plaintext:  ");
        char encrypt;
        for (int i = 0; message[i] != '\0'; i++)
        {
            if (message[i] >= 'a' && message[i] <= 'z')         // For small letters
            {
                encrypt = (int)(message [i]);                   //char to int
                encrypt -= 97;                                  // let's alphabet start from a = 0
                encrypt = (encrypt + k) % 26;                   // ensures a circle
                encrypt += 97;                                  // back to ascii
                message[i] = (char)(encrypt);                   // int to char
            }
            if (message[i] >= 'A' && message[i] <= 'Z')         //For capital letters
            {
                encrypt = (int)(message [i]);                   // char to int
                encrypt -= 65;                                  // let's alphabet start from A = 0
                encrypt = (encrypt + k) % 26;                   // ensures a circle
                encrypt += 65;                                  // back to ascii
                message[i] = (char)(encrypt);                   // int to char
            }
        }
        printf("ciphertext: %s\n", message);
        return 0;
    }
    else
    {
        printf("Usage: ./caesar key\n");                        // Shows error for negative characters
        return 1;
    }
}