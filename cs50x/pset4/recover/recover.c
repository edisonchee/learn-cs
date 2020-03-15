// recovers JPEGs from a forensic image

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) 
{
    // accept only 1 argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover image \n");
        return 1;
    }
    
    // recovered image counter
    int num_recovered = 0;
    char image_file_name[8];
    
    // open input file
    FILE *infile = fopen(argv[1], "r");
    // check if file is invalid
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    // prep output file
    FILE *outfile;
    
    // sizeof() returns unsigned type size_t
    size_t block_size = 512 * sizeof(uint8_t);

    // initialise buffer for reading file stream
    uint8_t *buffer = malloc(block_size);

    // running process to read till EOF
    while (1)
    {
        // end process if EOF
        if (fread(buffer, block_size, 1, infile) != 1)
        {
            break;
        }

        // check if jpeg is found
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous open file once first jpeg found
            if (num_recovered > 0)
            {
                fclose(outfile);
            }

            // set outfile name
            if (num_recovered < 10)
            {
                sprintf(image_file_name, "00%i.jpg", num_recovered);
            }
            else
            {
                sprintf(image_file_name, "0%i.jpg", num_recovered);
            }

            // open output file
            outfile = fopen(image_file_name, "w");
            if (outfile == NULL)
            {
                fclose(outfile);
                fprintf(stderr, "Could not create %s.\n", image_file_name);
                return 3;
            }
            
            // increment recovered image count
            num_recovered++;
        }
        // begin writing once first jpeg is found
        if (num_recovered > 0)
        {
            fwrite(buffer, sizeof(unsigned char), 512, outfile);
        }
    }
    
    // deallocate memory
    free(buffer);

    // close infile
    fclose(infile);

    // close outfile
    fclose(outfile);

    return 0;
}
