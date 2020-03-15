// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

typedef struct NODE
{
    bool is_word;
    struct NODE *children[27];
}
NODE;

NODE *root;
FILE *dictionary_file;
int dictionary_size;

// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);
bool recurse(NODE *node, const char *string, int index);
bool check_recursion(NODE *node, const char *string, int index);
void free_recursive(NODE *node);

#endif // DICTIONARY_H
