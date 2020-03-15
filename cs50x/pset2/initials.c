#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool IsLowerCase(char c);
void ToUpperCase(char c);

int main(void)
{
    string s = GetString();
    int len = strlen(s);

    ToUpperCase(s[0]);

    for (int i = 0; i < len; i++) {
        if (s[i] == ' ') {
            ToUpperCase(s[i + 1]);
        }
    }
    printf("\n");
}

bool IsLowerCase(char c) {
    if (c < 123 && c > 96) {
        return true;
    } else {
        return false;
    }
}

void ToUpperCase(char c) {
    if (IsLowerCase(c)) {
        printf("%c", c - 32);
    } else {
        printf("%c", c);
    }
}