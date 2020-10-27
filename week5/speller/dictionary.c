// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 99999;

// initialize word counter and hash value
int word_count = 0;
int hash_value;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // hash word to obtain hash value
    hash_value = hash(word);

    // access linked list at that index in hash table
    node *cursor = table[hash_value];

    // traverse linked list looking for the word
    while (cursor != NULL)
    {
        // compare words case-insensitively
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // source: djb2 by dan bernstein (http://www.cse.yorku.ca/~oz/hash.html)
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);
    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE *dict = fopen(dictionary, "r");

    // check if return value is null
    if (dict == NULL)
    {
        return false;
    }

    // loop through read strings from dictionary
    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // create new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // copy word into word field of the node
        strcpy(n->word, word);

        // hash word
        hash_value = hash(word);

        // insert node into hash table
        // set new node's pointer to the first element in the linked list
        n->next = table[hash_value];
        // set head pointer to new node
        table[hash_value] = n;
        word_count++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }

    return false;
}
