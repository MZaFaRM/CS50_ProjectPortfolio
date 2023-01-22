#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

int BLOCK_SIZE = 512;

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Checks for RAW file during execution
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Opens the given RAW file
    FILE *file = fopen(argv[1], "r");

    // Checks if RAW file is openable
    if (file == NULL)
    {
        printf("Could not open the file. \n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];

    int i = -1;

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // Checks for Headers
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xef) == 0xe0)
        {
            // Increment for file name
            i++;
        }

        // Starts storing from the first Header
        if (i >= 0)
        {

            // Array for storing file name
            char name[20];

            // Dynamic file name
            sprintf(name, "%.3d.jpg", i);

            //Creating file with the given name
            FILE *output = fopen(name, "a");

            // Writing data into file from card.raw
            fwrite(buffer, 1, BLOCK_SIZE, output);

            // File close
            fclose(output);

        }
    }
    fclose(file);
}