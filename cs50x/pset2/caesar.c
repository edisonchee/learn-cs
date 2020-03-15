#include <cs50.h>
#include <stdio.h>
#include <string.h>

void RotateUpperCase(char c, int key);
void RotateLowerCase(char c, int key);

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2) {
        fprintf(stdout, "Please provide key");
        return 1;
    }

    // secret key
    int input = atoi(argv[1]);
    int key = input % 26;
    
    // get ciphertext
    string s = GetString();

    // get length of string
    int len = strlen(s);

    // loop through each char
    for (int i = 0; i < len; i++) {
        // check if uppercase
        if (s[i] > 64 && s[i] < 91) {
            RotateUpperCase(s[i], key);
        } else if (s[i] > 96 && s[i] < 123) {
            RotateLowerCase(s[i], key);
        } else {
            printf("%c", s[i]);
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