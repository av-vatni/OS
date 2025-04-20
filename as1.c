#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("\nCommand line arguments are less than 3....\n");
        printf("Exiting program\n");
        return 0;
    }

    const char *pattern = argv[1];
    const char *filename = argv[2];

    FILE *fptr;
    fptr = fopen("demo.txt", "r");
    if (fptr == NULL)
    {
        printf("The file is not opened. The program exits...");
        return 0;
    }

    else
    {
        char line[1024];
        int line_number = 0;

        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            line_number += 1;

            if (strstr(line, pattern) != NULL)
            {
                printf("Line %d : %s", line_number, line);
            }
        }
    }

    printf("\n");

    fclose(fptr);
    return 0;
}