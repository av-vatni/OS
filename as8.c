#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate the total head movement using FIFO
int fifo(int requests[], int n, int head) {
    int total_head_movement = 0;
    for (int i = 0; i < n; i++) {
        total_head_movement += abs(requests[i] - head);
        head = requests[i];
    }
    return total_head_movement;
}

// Function to calculate the total head movement using SSTF
int sstf(int requests[], int n, int head) {
    int total_head_movement = 0;
    int completed[n];
    for (int i = 0; i < n; i++) {
        completed[i] = 0; // Mark all requests as unprocessed
    }

    for (int i = 0; i < n; i++) {
        int min_distance = __INT_MAX__;
        int closest_index = -1;

        // Find the closest unprocessed request
        for (int j = 0; j < n; j++) {
            if (!completed[j] && abs(requests[j] - head) < min_distance) {
                min_distance = abs(requests[j] - head);
                closest_index = j;
            }
        }

        // Move to the closest request
        total_head_movement += min_distance;
        head = requests[closest_index];
        completed[closest_index] = 1; // Mark the request as processed
    }

    return total_head_movement;
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to calculate the total head movement using SCAN
int scan(int requests[], int n, int head, int disk_size) {
    int total_head_movement = 0;
    int sorted_requests[n];

    for (int i = 0; i < n; i++) {
        sorted_requests[i] = requests[i];
    }

    qsort(sorted_requests, n, sizeof(int), compare);

    // First, go to the right (higher values)
    for (int i = 0; i < n; i++) {
        if (sorted_requests[i] >= head) {
            total_head_movement += abs(sorted_requests[i] - head);
            head = sorted_requests[i];
        }
    }

    // Then move to the end of the disk (if not already there)
    if (head != disk_size - 1) {
        total_head_movement += abs((disk_size - 1) - head);
        head = disk_size - 1;
    }

    // Then reverse and process the remaining (left side)
    for (int i = n - 1; i >= 0; i--) {
        if (sorted_requests[i] < requests[0]) break;
        if (sorted_requests[i] < requests[0] || sorted_requests[i] < head) {
            total_head_movement += abs(sorted_requests[i] - head);
            head = sorted_requests[i];
        }
    }
    return total_head_movement;
}

// Function to calculate the total head movement using CSCAN
int cscan(int requests[], int n, int head, int disk_size) {
    int total_head_movement = 0;
    int sorted_requests[n];

    for (int i = 0; i < n; i++) {
        sorted_requests[i] = requests[i];
    }

    qsort(sorted_requests, n, sizeof(int), compare);

    // Service the requests on the right of the head
    for (int i = 0; i < n; i++) {
        if (sorted_requests[i] >= head) {
            total_head_movement += abs(sorted_requests[i] - head);
            head = sorted_requests[i];
        }
    }

    // Move from end to beginning (jump)
    if (head != disk_size - 1) {
        total_head_movement += abs((disk_size - 1) - head);
        head = 0;
        total_head_movement += disk_size - 1;  // jump from end to start
    } else {
        head = 0;
        total_head_movement += disk_size - 1;
    }

    // Service the requests on the left
    for (int i = 0; i < n; i++) {
        if (sorted_requests[i] < head) {
            total_head_movement += abs(sorted_requests[i] - head);
            head = sorted_requests[i];
        }
    }

    return total_head_movement;
}


int main() {
    int n, head;

    // number of requests
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    int requests[n];

    // requests
    printf("Enter the disk requests: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    // initial head position
    printf("Enter the initial head position: ");
    scanf("%d", &head);

    // total head movement for FIFO
    int fifo_movement = fifo(requests, n, head);
    printf("Total head movement using FIFO: %d\n", fifo_movement);

    // total head movement for SSTF
    int sstf_movement = sstf(requests, n, head);
    printf("Total head movement using SSTF: %d\n", sstf_movement);

    // disk size for SCAN and CSCAN
    int disk_size;
    printf("Enter the disk size: ");
    scanf("%d", &disk_size);

    // total head movement for SCAN
    int scan_movement = scan(requests, n, head, disk_size);
    printf("Total head movement using SCAN: %d\n", scan_movement);

    // total head movement for CSCAN
    int cscan_movement = cscan(requests, n, head, disk_size);
    printf("Total head movement using CSCAN: %d\n", cscan_movement);

    return 0;
}