#include <cs50.h>
#include <stdio.h>

// function prototypes
float AskAmount(void);
int GetChange(int xRounded);

int main(void)
{
    // number of change
    int c = 0;
    
    // ask user for change required
    float x = AskAmount();
    
    // round up x, cast to int, i.e. convert user input to cents
    int xRounded = (int) (x * 100 + 0.5);
    
    c = GetChange(xRounded);
    
    printf("%i\n", c);
    
}

/**
* Ask user for height of half-pyramid.
*/

float AskAmount(void)
{
    float x;
    do
    {
        printf("How much change is owed: ");
        x = GetFloat();
    }
    while (x < 0);
    return x;
}

/**
* Greedy algorithm for figuring out how much change.
*/

int GetChange(int xRounded)
{
    int c = 0;

    while (xRounded != 0)
    {
        while (xRounded > 24)
        {
            xRounded -= 25;
            c++;
        }
        
        while (xRounded > 9)
        {
            xRounded -= 10;
            c++;
        }
        
        while (xRounded > 4)
        {
            xRounded -= 5;
            c++;
        }
        
        while (xRounded > 0)
        {
            xRounded -=1;
            c++;            
        }
    }
    
    return c;
}