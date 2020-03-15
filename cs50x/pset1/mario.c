#include <cs50.h>
#include <stdio.h>

// function prototypes
int AskHeight(void);
void PrintHalfPyramid(int h);

int main(void)
{
    // ask user for height of half-pyramid
    int h = AskHeight();
    
    //print half-pyramid
    PrintHalfPyramid(h);
}

/**
* Ask user for height of half-pyramid.
*/

int AskHeight(void)
{
    int h;
    do
    {
        printf("height: ");
        h = GetInt();
    }
    while (h < 0 || h > 23);
    return h;
}

/**
* Prints half-pyramid.
*/

void PrintHalfPyramid(int height) {
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = height; j > 0; j--)
        {
            if (j > i)
            {
                printf(" ");
            } else
            {
                printf("#");
            }
        };
        printf("#\n");
    }
}