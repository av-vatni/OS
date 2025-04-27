#include <stdio.h>
#include <stdbool.h>

// Function to implement FIFO Page Replacement Algorithm
void fifoPageReplacement(int pages[], int n, int capacity) {
    int frame[capacity]; // Array to hold pages in memory
    int front = 0, pageHits = 0, pageFaults = 0;

    // Initialize the frame with -1 (indicating empty slots)
    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        bool found = false;
        // Check if the page is already in the frame
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                pageHits++;
                break;
            }
        }
        // If the page is not found, replace the oldest page (FIFO logic)
        if (!found) {
            frame[front] = pages[i];
            front = (front + 1) % capacity; // Update the front pointer
            pageFaults++; // Increment page fault count
        }
        // Print the current state of the frame
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) {
            if (frame[j] != -1)
                printf("%d ", frame[j]);
            else
                printf("- ");
        }
        printf("\n");
    }
    // Print the total number of page faults
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits  : %d\n", pageHits);
}

// Function to implement LRU Page Replacement Algorithm
void  lruPageReplacement(int pages[], int n, int capacity) {
    int frame[capacity]; // Array to hold pages in memory
    int recent[capacity]; // Array to track the last used time of each page
    int pageFaults = 0, pageHits = 0;

    // Initialize the frame and recent arrays with -1
    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
        recent[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        bool found = false;

        // Check if the page is already in the frame
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                pageHits++;
                recent[j] = i; // Update the last used time
                break;
            }
        }

        // If the page is not found, replace the least recently used page
        if (!found) {
            int lruIndex = 0;
            // Find the index of the least recently used page
            for (int j = 1; j < capacity; j++) {
                if (recent[j] < recent[lruIndex]) {
                    lruIndex = j;
                }
            }

            frame[lruIndex] = pages[i]; // Replace the LRU page
            recent[lruIndex] = i; // Update the last used time
            pageFaults++; // Increment page fault count
        }

        // Print the current state of the frame
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) {
            if (frame[j] != -1)
                printf("%d ", frame[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    // Print the total number of page faults
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits  : %d\n", pageHits);

}

// Function to implement Optimal Page Replacement Algorithm
void optimalPageReplacement(int pages[], int n, int capacity) {
    int frame[capacity]; // Array to hold pages in memory
    int pageFaults = 0, pageHits = 0;

    // Initialize the frame with -1 (indicating empty slots)
    for (int i = 0; i < capacity; i++) {
        frame[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        bool found = false;

        // Check if the page is already in the frame
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == pages[i]) {
                found = true;
                pageHits++;
                break;
            }
        }

        // If the page is not found, replace a page using the Optimal algorithm
        if (!found) {
            int replaceIndex = -1, farthest = i + 1;

            // Find the page that will not be used for the longest time
            for (int j = 0; j < capacity; j++) {
                int nextUse = -1;
                for (int k = i + 1; k < n; k++) {
                    if (frame[j] == pages[k]) {
                        nextUse = k;
                        break;
                    }
                }

                if (nextUse == -1) { // Page is not used again
                    replaceIndex = j;
                    break;
                } else if (nextUse > farthest) {
                    farthest = nextUse;
                    replaceIndex = j;
                }
            }

            // Replace the page
            if (replaceIndex == -1) {
                replaceIndex = 0; // Default to the first frame if no replacement index is found
            }
            frame[replaceIndex] = pages[i];
            pageFaults++; // Increment page fault count
        }

        // Print the current state of the frame
        printf("Frame: ");
        for (int j = 0; j < capacity; j++) {
            if (frame[j] != -1)
                printf("%d ", frame[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    // Print the total number of page faults
    printf("Total Page Faults: %d\n", pageFaults);
}

int main() {
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2}; // Reference string
    int n = sizeof(pages) / sizeof(pages[0]); // Number of pages
    int capacity = 3; // Number of frames

    // Run FIFO Page Replacement
    printf("FIFO Page Replacement:\n");
    fifoPageReplacement(pages, n, capacity);

    // Run LRU Page Replacement
    printf("\nLRU Page Replacement:\n");
    lruPageReplacement(pages, n, capacity);

    // Run Optimal Page Replacement
    printf("\nOptimal Page Replacement:\n");
    optimalPageReplacement(pages, n, capacity);

    return 0;
}