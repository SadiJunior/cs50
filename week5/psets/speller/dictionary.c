// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

// Functions
bool free_list(node *head);
unsigned int get_list_size(node *head);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_key = hash(word);

    node *iterator = table[hash_key];

    // Iterates through the Hash list.
    while (iterator != NULL)
    {
        // Compares the two strings
        if (strcasecmp(word, iterator->word) == 0)
        {
            return true;
        }

        iterator = iterator->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Checks the two first letters.
    int first_char = toupper(word[0]) - 'A';
    int second_char = strlen(word) > 1 ? toupper(word[1]) - 'A' : 0;

    // Multiplies the first one for 26, for getting the hash key
    return first_char * 26 + second_char;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Could not create node.\n");
            free(new_node);
            return false;
        }

        strcpy(new_node->word, word);
        new_node->next = NULL;

        int hash_key = hash(new_node->word);

        // If there is no element in the given position of the Hash Table
        if (table[hash_key] == NULL)
        {
            table[hash_key] = new_node;
        }
        // If there is already a linked list in the given position
        else
        {
            // Points the new node to the current head (making it the head)
            new_node->next = table[hash_key];
            // Puts the new node in the head of the Hash Table position
            table[hash_key] = new_node;
        }
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int size = 0;

    // Iterates through the Hash Table
    for (int i = 0; i < N; i++)
    {
        // If there is a linked list in the given Hash Table position
        if (table[i] != NULL)
        {
            size += get_list_size(table[i]);
        }
    }

    return size;
}

// Gets the size of the list specified by head
unsigned int get_list_size(node *head)
{
    if (head == NULL)
    {
        return 0;
    }

    int list_size = 0;

    node *iterator = head;

    while (iterator != NULL)
    {
        list_size++;
        iterator = iterator->next;
    }

    return list_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // If there is a linked list in the given Hash Table position
        if (table[i] != NULL)
        {
            if (!free_list(table[i]))
            {
                return false;
            }

            table[i] = NULL;
        }
    }

    return true;
}

// Frees the linked list specified by head.
bool free_list(node *head)
{
    if (head == NULL)
    {
        return false;
    }

    node *iterator = head;

    // While it doesn't get to the end of the list.
    while (iterator != NULL)
    {
        // Initializes a temp node for being removed
        node *temp = iterator;

        // Sets the current node to be the next one
        iterator = iterator->next;

        // Frees the temp node
        free(temp);
    }

    return true;
}