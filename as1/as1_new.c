#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void simulate_cp(const char *src, const char *dest) {
    FILE *fsrc = fopen(src, "r");
    FILE *fdest = fopen(dest, "w");

    if (!fsrc || !fdest) {
        perror("Error opening files");
        exit(1);
    }

    char ch;
    while ((ch = fgetc(fsrc)) != EOF) {
        fputc(ch, fdest);
    }

    fclose(fsrc);
    fclose(fdest);
    printf("File copied successfully from '%s' to '%s'\n", src, dest);
    exit(0);
}

void simulate_grep(const char *pattern, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            printf("Match found: %s", line);
            found = 1;
        }
    }

    if (!found)
        printf("Pattern '%s' not found in file '%s'\n", pattern, filename);

    fclose(file);
    exit(0);
}

int main() {
    int choice;
    char src[100], dest[100], pattern[100], filename[100];
    pid_t pid;

    printf("Parent process started (PID: %d)\n", getpid());
    printf("Choose an option:\n1. Simulate 'cp' command\n2. Simulate 'grep' command\nEnter choice: ");
    scanf("%d", &choice);

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child process started (PID: %d)\n", getpid());

        if (choice == 1) {
            printf("Enter source file: ");
            scanf("%s", src);
            printf("Enter destination file: ");
            scanf("%s", dest);
            simulate_cp(src, dest);
        }
        else if (choice == 2) {
            printf("Enter filename: ");
            scanf("%s", filename);
            printf("Enter pattern to search: ");
            scanf("%s", pattern);
            simulate_grep(pattern, filename);
        }
        else {
            printf("Invalid choice in child\n");
            exit(1);
        }
    } else {
        wait(NULL);
        printf("\nChild process finished. Back in parent (PID: %d)\n", getpid());
    }

    return 0;
}
