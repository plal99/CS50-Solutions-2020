// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 100;

// Number of words in the dictionary
unsigned int count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int loc = hash(word);
    node *cursor = table[loc];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }

    return false;
}

// Hashes word to a number (self made)
unsigned int hash(const char *word)
{
    int sum = 0, len = strlen(word);
    
    for (int i = 0 ; i < len ; i++)
    {
        sum += (int)tolower(word[i]);
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Dictionary not found\n");
        return 1;
    }
    char temp[LENGTH + 1];
    unsigned int loc;
    while (fscanf(file, "%s", temp) != EOF)
    {
        node *newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            unload();
            return false;
        }
        strcpy(newNode->word, temp);
        newNode->next = NULL;
        loc = hash(temp);
        
        newNode->next = table[loc];
        table[loc] = newNode;
        count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp, *cursor;
    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        cursor = tmp;
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
        
    }
    return true;
}
