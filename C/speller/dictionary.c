// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int FIRST_LETTERS = 3;

int dictionary_size;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 27 * 27;

// Hash table
node *table[N];

int size_value = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int position = hash(word);

    node *finder = table[position];

    for (int i = 0; finder != NULL; i++)
    {
        if (!strcasecmp(finder->word, word))
        {
            return true;
        }
        finder = finder->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    int value = (int)(toupper(word[0]) - 'A');

    for (int i = 1; i < FIRST_LETTERS; i++)
    {
        value *= 27;

        if (word[i] == '\0')
        {
            return value;
        }

        value += (int)(toupper(word[i]) - '@');
    }

    return value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dictionary_pointer = fopen(dictionary, "r");

    char buffer[LENGTH + 1];

    if (!(dictionary_pointer == NULL))
    {
        // Stores the current word into a node
        while (fscanf(dictionary_pointer, "%s", buffer) != EOF)
        {
            // Allocates a memory for the current word
            node *current_word = malloc(sizeof(node));

            // Copies the word from buffer to node
            strcpy(current_word->word, buffer);

            // Error message if error in memory allocation
            if (current_word == NULL)
            {
                return 1;
            }

            // The current node to the first element of the table
            current_word->next = table[hash(current_word->word)];

            // Calculates the size of dictionary for size function
            size_value++;

            // The table points to the current word
            table[hash(current_word->word)] = current_word;
        }

        // Close the file if all goes well
        fclose(dictionary_pointer);

        // Return true if all goes well
        return true;
    }

    // Unable to open file
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Value provided by hash function
    return size_value;
}

void sub_unload(node *current_node)
{
    // Base case
    // If current node is pointing towards nothing free it
    if (current_node->next == NULL)
    {
        free(current_node);
        return;
    }

    // If it's pointing towards something free it
    sub_unload(current_node->next);

    // Then free it
    free(current_node);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (!(table[i] == NULL))
        {
            sub_unload(table[i]);
        }
    }
    return true;
}
