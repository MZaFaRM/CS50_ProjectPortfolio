#include <cs50.h>
#include <stdio.h>
#include<ctype.h>
#include<string.h>

int main(int argc, string argv[])
{
    string ctxt = argv[1];
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");                                  // Checks if key is given
        return 1;
    }
    if (strlen(ctxt) < 26)
    {
        printf("Key must contain 26 characters.\n");                            // Checks if key is 26 characters
        return 1;
    }
    for (int i = 0; ctxt[i] != '\0'; i++)
    {
        if (isalpha(ctxt[i]) == 0)
        {
            printf("Key must only contain Alphabetic characters.\n");           // Ensures key is alphabetic
            return 1;
        }
    }
    for (int i = 0; ctxt[i] != '\0'; i++)
    {
        for (int j = i + 1; ctxt[j] != '\0'; j++)
        {
            if (ctxt[i] == ctxt[j])
            {
                printf("Key must not contain repetitive characters.\n");        // Checks for repetitions in key
                return 1;
            }
        }
    }
    string message = get_string("plain text:  ");                               // Finally, takes the message
    for (int i = 0; message[i] != '\0'; i++)
    {
        char c = message[i];                                                    // Passes a letter to c
        int n = (int)(c);
        if (islower(c))
        {
            n -= 97;                                                            // Starts lower case from 0, not 97
            c = tolower(ctxt[n]);                                               // replaces c with ctext character
        }
        else if (isupper(c))
        {
            n -= 65;                                                            // Starts upper case from 0, not 65
            c = toupper(ctxt[n]);                                               // replaces c with ctext character
        }
        message[i] = c;                                                         // Transformed letter replaces orginal letter
    }
    printf("ciphertext: %s\n", message);                                        // Output
    return 0;
}