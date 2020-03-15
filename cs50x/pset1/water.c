#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ask user for number of minutes
    printf("minutes: ");
    int m = GetInt();
    
    // calculate number of bottles
    int b = m * 12;
    
    printf("bottles: %i\n", b);
}