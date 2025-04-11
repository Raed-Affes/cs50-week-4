#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Each JPEG block is 512 bytes
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover filename\n");
        return 1;
    }

    // Open memory card file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    uint8_t buffer[BLOCK_SIZE];
    FILE *img = NULL;
    int file_count = 0;
    char filename[8];

    // Read blocks of 512 bytes
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, file))
    {
        // Check for JPEG signature
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If already writing a JPEG, close it
            if (img != NULL)
            {
                fclose(img);
            }

            // Create new JPEG filename and open file
            sprintf(filename, "%03i.jpg", file_count++);
            img = fopen(filename, "w");
        }

        // If currently writing a JPEG, write the block
        if (img != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, img);
        }
    }

    // Cleanup
    if (img != NULL)
    {
        fclose(img);
    }

    fclose(file);
    return 0;
}
