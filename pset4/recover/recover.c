#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t BYTE; // making byte datatype

int main(int argc, char *argv[])
{
    if (argc != 2) // if commandline arguments are more or less than one filename
    {
        printf("Usage: ./recover image");
        return 1;
    }
    FILE *file = NULL;
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Forensic image cannot be opened"); // if the file cannot be opened, then the pointer would remain NULL
        return 1;
    }
    BYTE buffer[512]; // a block buffer
    char name[8]; // sprintf name variable
    FILE *img = NULL; // output pointer
    int num = 0;
    while (true)
    {
        int size = fread(buffer, sizeof(BYTE), 512, file);
        if (size == 0 && feof(file)) // to check EOF
        {
            break;
        }
        bool condition = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
        if (condition == true && img != NULL) // condition1
        {
            fclose(img);
            num++;
        }
        if (condition == true) // condition2
        {
            sprintf(name, "%03i.jpg", num);
            img = fopen(name, "w");
        }
        if (img != NULL) // condition3
        {
            fwrite(buffer, sizeof(BYTE), size, img);
        }
    }
    fclose(img); // to avoid segmentation error at the end of the forensic image
    fclose(file);
}
