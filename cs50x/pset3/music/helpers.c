// Helper functions for music

#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "helpers.h"

int distance(char note);

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    char numeratorStr[2];
    numeratorStr[0] = fraction[0];
    numeratorStr[1] = '\0';
    char denominatorStr[2];
    denominatorStr[0] = fraction[2];
    denominatorStr[1] = '\0';
    int numerator = atoi(numeratorStr);
    int denominator = atoi(denominatorStr);
    if (denominator == 2)
    {
        return numerator * 4;
    }
    else if (denominator == 4)
    {
        return numerator * 2;
    }
    else if (denominator == 8)
    {
        return numerator * 1;
    }
    else
    {
        fprintf(stderr, "Error\n");
        return 0;
    }
};

// Calculates frequency (in Hz) of a note
// 2^(n/12) × 440
int frequency(string note)
{
    int n;
    int octave;
    double power;
    // Get distance from the A note
    switch (note[0])
    {
        case 65:
            n = 0;
            break;
        case 66:
            n = 2;
            break;
        case 67:
            n = -9;
            break;
        case 68:
            n = -7;
            break;
        case 69:
            n = -5;
            break;
        case 70:
            n = -4;
            break;
        case 71:
            n = -2;
            break;
        default:
            fprintf(stderr, "Error: Not a valid note.\n");
            n = 0;
            break;
    }
    // Check for accidentals
    if (strlen(note) == 2)
    {
        octave = (note[1] - 48) - 4;
        power = (octave * 12) + n;
        return round(pow(2, power / 12) * 440);
    }
    else
    {
        octave = (note[2] - 48) - 4;
        if (note[1] == 35)
        {
            n++;
        }
        else
        {
            n--;
        }
        power = (octave * 12) + n;
        return round(pow(2, power / 12) * 440);
    }
};

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    return false;
};
