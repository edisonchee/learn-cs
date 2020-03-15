// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dictionary.h"

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    return check_recursion(root, word, 0);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // initialise root
    root = calloc(1, sizeof(NODE));
    // temporary storage for string
    char string[LENGTH + 1];
    
    // open dictionary file
    dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }
    
    // set dictionary size counter to 0
    dictionary_size = 0;

    // while successfully matched
    while (fscanf(dictionary_file, "%s", string) != EOF)
    {
        if (recurse(root, string, 0))
        {
            dictionary_size++;
        }
        else
        {
            return false;
        }
    }
    
    if (root == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        return dictionary_size;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    if (root != NULL)
    {
        free_recursive(root);
    }

    if (fclose(dictionary_file) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// trie recursion
bool recurse(NODE *node, const char *string, int index)
{
    int pos;
    char c = string[index];

    if (c == '\'')
    {
        pos = 26;
    }
    else
    {
        pos = c - 97;                
    }

    if (node->children[pos] == NULL)
    {
        node->children[pos] = calloc(1, sizeof(NODE));
    }

    index++;
    if (index == strlen(string))
    {
        node->children[pos]->is_word = true;
        return true;
    }
    
    return recurse(node->children[pos], string, index);
}

// check recursion
bool check_recursion(NODE *node, const char *string, int index)
{
    int pos;
    char c = string[index];

    if (c == '\'')
    {
        pos = 26;
    }
    else if (c > 64 && c < 91)
    {
        pos = c - 97 + 32;
    }
    else
    {
        pos = c - 97;                
    }
    
    if (node->children[pos] == NULL)
    {
        return false;
    }

    index++;
    if (index == strlen(string))
    {
        return node->children[pos]->is_word;
    }
    
    return check_recursion(node->children[pos], string, index);
}

// free linked list memory
void free_recursive(NODE *node)
{
    // return immediately if node is null
    if (node == NULL)
    {
        return;
    }

    // loop through every member
    for (int i = 0; i < 27; i++)
    {
        if (node->children[i] != NULL)
        {            
            free_recursive(node->children[i]);
        }
    }
    // free memory allocated for node
    free(node);
    // free pointer
    node = NULL;
}
