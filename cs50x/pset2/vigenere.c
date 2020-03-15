#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void RotateUpperCase(char c, int key);
void RotateLowerCase(char c, int key);

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2) {
        fprintf(stdout, "Please provide key");
        return 1;
    }

    // Key
    char *key = argv[1];
    size_t key_len = strlen(key);

    // Complain if any non-alphabetical letter
    for (int i = 0; i < key_len; i++) {
        if (!isalpha(key[i])) {
            fprintf(stdout, "Please input only letters A-Z, a-z.");
            return 1;
        }
    };

    // Make cipher key
    for (int i = 0; i < key_len; i++) {
        if (key[i] > 64 && key[i] < 91) {
            // Uppercase
            key[i] -= 65;
        } else {
            // Lowercase
            key[i] -= 97;
        }
    };

    int counter = 0;

    // get ciphertext
    string cipher_text = GetString();
    size_t text_len = strlen(cipher_text);

    for (int i = 0; i < text_len; i++) {
        if (counter == key_len) {
            counter = 0;
        }
        if (cipher_text[i] > 64 && cipher_text[i] < 91) {
            RotateUpperCase(cipher_text[i], key[counter] % 26);
            counter++;
        } else if (cipher_text[i] > 96 && cipher_text[i] < 123) {
            RotateLowerCase(cipher_text[i], key[counter] % 26);
            counter++;
        } else {
            printf("%c", cipher_text[i]);
        }
    }
    printf("\n");
    return 0;
}

void RotateUpperCase(char c, int key) {
    int distance = 90 - c;
    if (key < distance || key == distance) {
        printf("%c", c + key);
    } else {
        printf("%c", 64 + (key - distance));
    }
}

void RotateLowerCase(char c, int key) {
    int distance = 122 - c;
    if (key < distance || key == distance) {
        printf("%c", c + key);
    } else {
        printf("%c", 96 + (key - distance));
    }
}