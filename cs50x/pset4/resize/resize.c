// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n-factor infile outfile\n");
        return 1;
    }

    // store n-factor
    int nFactor = atoi(argv[1]);
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    printf("n-factor: %i\n", nFactor);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine old padding for scanlines
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // initialize variables for new file's BITMAPFILEHEADER and BITMAPINFOHEADER
    BITMAPFILEHEADER outbf = bf;
    BITMAPINFOHEADER outbi = bi;
    
    // calculate resized image width and height in pixels
    outbi.biWidth *= nFactor;
    outbi.biHeight *= nFactor;
    
    // determine new padding for scanlines
    int newPadding = (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // include new padding to calculate number of bytes
    outbi.biSizeImage = (outbi.biWidth * abs(outbi.biHeight) * sizeof(RGBTRIPLE)) + (newPadding * abs(outbi.biHeight));
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER 
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // array for pixels in current scanline
        RGBTRIPLE row[bi.biWidth];
        
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // store pixel in array
            row[j] = triple;
        }

        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);

        // resize vertically
        for (int k = 0; k < nFactor; k++)
        {
            // resize horizontally
            for (int l = 0; l < bi.biWidth; l++)
            {
                for (int m = 0; m < nFactor; m++)
                {
                    fwrite(&row[l], sizeof(RGBTRIPLE), 1, outptr);
                }
            }            
            // then add it back (to demonstrate how)
            for (int n = 0; n < newPadding; n++)
            {
                fputc(0x00, outptr);
            }
        }

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
