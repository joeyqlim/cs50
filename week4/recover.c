#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // check if input is valid
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open memory card
    FILE *input_file = fopen(argv[1], "r");
    if (argv[1] == NULL)
    {
        printf("Unable to open file: %s\n", argv[1]);
        return 1;
    }

    // REPEAT until end of card:
    // read the opened file into buffer
    typedef uint8_t BYTE; // create new type 'BYTE'
    typedef enum
    {
        true, false
    }
    boolean;
    BYTE buffer[512]; // initialize a buffer
    int file_counter = 0; // initialize file counter for naming jpegs
    FILE *output_file = NULL; // initialize file pointer for output file
    boolean jpeg_already_found = false; // initialize boolean

    while (fread(&buffer, 512, 1, input_file) == 1)
    {
        // check if this is the start of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if already found JPEG, close previous file and open new file
            if (jpeg_already_found == true)
            {
                fclose(output_file);
            }
            else
            {
                jpeg_already_found = true;
            }
            // if first JPEG, start writing first file in 000.jpg format
            char file_name[8]; // initialize string with 7 + 1 char
            sprintf(file_name, "%03i.jpg", file_counter);
            output_file = fopen(file_name, "w");
            file_counter++;
        }
        // if not JPEG and already found JPEG, keep writing to the file
        if (jpeg_already_found == true)
        {
            fwrite(&buffer, 512, 1, output_file);
        }
    }
    // once the end of the card is reached, close any remaining files
    if (output_file == NULL)
    {
        fclose(output_file);
    }
    if (input_file == NULL)
    {
        fclose(input_file);
    }
}
