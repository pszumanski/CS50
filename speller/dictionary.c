// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
unsigned int size_count = 0;
unsigned int hashval = 0;

// Hash table
node *table[N];
node *cursor;
node *deleter;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Check hash value for given word
    hashval = hash(word);
    // Point curosr at adequate linked list
    cursor = table[hashval];
    // Search untill word found or reached end of the list
    while (cursor->next != NULL && strcasecmp(cursor->word, word) != 0)
    {
        cursor = cursor->next;
    }
    // If end of the list, check if final word is correct
    if (strcasecmp(cursor->word, word) == 0)
    {
        cursor = NULL;
        return true;
    }
    else
    {
        cursor = NULL;
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // Give hash value
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //TODO
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    // Check for error
    if (dict == NULL)
    {
        return false;
    }
    // Create temp place to store word
    char *temp = malloc(sizeof(char) * (LENGTH + 1));
    // Initialize empty node
    node *n;
    // Read words until end of the file reached and store them
    while (fscanf(dict, "%s", temp) != EOF)
    {
        // Count size of the file
        size_count++;
        // Allocate memory for a node
        n = malloc(sizeof(node));
        // Check for error
        if (n == NULL)
        {
            return false;
        }
        // Store word in new node
        strcpy(n->word, temp);
        // Give hash value to word
        hashval = hash(n->word);
        // Point new node to start of linked list
        n->next = table[hashval];
        // Point start of linked list to new node
        table[hashval] = n;
        //printf("WORD LOADED: %s, HASH: %i\n", table[hashval]->word, hashval);
        // Reset pointer
        n = NULL;
    }
    free(temp);
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return size_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Go over each hash value
    for (int i = 0; i < N && size_count > 0; i++)
    {
        // Set cursor to where linked list for given hash should be
        cursor = table[i];
        // Untill end of list reached
        while (cursor != NULL)
        {
            // Create temporary pointer to current node
            deleter = cursor;
            // Move cursor to next node
            cursor = cursor->next;
            // Remove previous node
            free(deleter);
            // Track elements deleted
            size_count--;
        }
    }
    // Check if all deleted
    if (size_count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
