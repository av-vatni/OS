#include<stdio.h>
#define MAX 100

void displayAllocation(int processSize[], int allocation[], int blockSize[], int n, int m) {
    printf("\nProcess No.\tProcess Size\tBlock No.\tFragmented Memory\n");
    for (int i = 0; i < n; i++) {
        if (allocation[i] == -1)
            printf(" %d\t\t%d\t\tNot Allocated\t-\n", i + 1, processSize[i]);
        else
            printf(" %d\t\t%d\t\t%d\t\t%d\n", i + 1, processSize[i], allocation[i], blockSize[allocation[i] - 1]);
    }
}

// First Fit
void firstFit(int blockSize[], int m, int processSize[], int n){
    int allocation[n];
    for(int i=0; i<n; i++){
        allocation[i] = -1;
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(blockSize[j] >= processSize[i]){
                allocation[i] = j+1;
                blockSize[j] -= processSize[i];
            }
        }
    }
    displayAllocation(processSize, allocation, blockSize, n, m);
}

// Best Fit
void bestFit(int blockSize[], int m, int processSize[], int n){
    int allocation[n];
    for(int i=0; i<n; i++){
        allocation[i] = -1;
    }
    for(int i=0;i<n; i++){
        int bestIndex = -1;
        for(int j=0; j<m; j++){
            if((blockSize[j]>= processSize[i]) && (bestIndex == -1 || blockSize[j] < blockSize[bestIndex])){
                bestIndex = j;
            }
        }
        if(bestIndex!= -1){
            allocation[i] = bestIndex + 1;
            blockSize[bestIndex] -= processSize[i];
        }
    }
    displayAllocation(processSize, allocation, blockSize, n, m);
}

void worstFit(int blockSize[], int m, int processSize[], int n){
    int allocation[n];
    for(int i=0; i<n; i++){
        allocation[i] = -1;
    }
    for(int i=0; i<n; i++){
        int worstIdx = -1;
        for(int j=0; j<m; j++){
            if((blockSize[i] >= processSize[i]) && worstIdx == -1 || blockSize[j] > blockSize[worstIdx]){
                worstIdx = j;
            }
        }
        if(worstIdx != -1){
            allocation[i] = worstIdx + 1;
            blockSize[worstIdx] -= processSize[i];
        }
    }
    displayAllocation(processSize, allocation, blockSize, n, m);
}

void nextFit(int blockSize[], int m, int processSize[], int n){
    int allocation[n];
    for(int i=0; i<n; i++){
        allocation[i] =  -1;
    }
    int lastAllocated = 0;
    for(int i=0; i<n; i++){
        int j = lastAllocated;
        do{
            if(blockSize[j] >= processSize[i]){
                allocation[i] = j+1;
                blockSize[j] -= processSize[i];
                lastAllocated = j;
                break;
            }
            j = (j+1) % m;    
        }while(j != lastAllocated);
    }
    displayAllocation(processSize, allocation, blockSize, n, m);
}

int main() {
    int m, n, choice;
    int blockSize[MAX], processSize[MAX], blockCopy[MAX];

    printf("Enter the number of memory blocks: ");
    scanf("%d", &m);
    printf("Enter sizes of %d memory blocks: ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &blockSize[i]);

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter sizes of %d processes: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &processSize[i]);

    do {
        for (int i = 0; i < m; i++)
            blockCopy[i] = blockSize[i]; // Reset block sizes

        printf("\nChoose Memory Allocation Strategy:\n");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Next Fit\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                firstFit(blockCopy, m, processSize, n);
                break;
            case 2:
                bestFit(blockCopy, m, processSize, n);
                break;
            case 3:
                worstFit(blockCopy, m, processSize, n);
                break;
            case 4:
                nextFit(blockCopy, m, processSize, n);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice! Please enter again.\n");
        }
    } while (choice != 5);

    return 0;
}