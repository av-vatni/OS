#include <stdio.h>
#include <stdbool.h>

#define MAX 10
int P, R; //Processes and Requests

// C]alculating need matrix
void calcNeedMat(int need[MAX][MAX], int max[MAX][MAX],  int alloc[MAX][MAX]){
    for(int i=0; i<P; i++){
        for(int j=0; j<R; j++){
            need[i][j] = max[i][j] -alloc[i][j];
        }
    }
}

void findSafeSeq(int avail[], int max[][MAX], int alloc[][MAX], bool finish[], int work[], int safeSeq[], int index){
    if(index == P){
        printf("Safe sequenece: ");
        for(int i=0; i<P; i++){
            printf("P%d", safeSeq[i]);
        }
        printf("\n");
    }

    for(int i=0; i<P; i++){
        if(!finish[i]){
            bool canRun = true;
            for(int j=0; j<R; j++){
                if(max[i][j] - alloc[i][j] > work[j]){
                    canRun = false;
                    break;
                }
            }

            if(canRun){
                for(int j=0; j<R; j++){
                    work[j] += alloc[i][j];
                }
                finish[i] = true;

                safeSeq[index] = i;
                findSafeSeq(avail,max,alloc,finish,work,safeSeq,index+1);

                for(int j=0; j<R; j++){
                    work[j] -= alloc[i][j];
                }
                finish[i] = false;
            }
        }
    }
}

int main() {
    int avail[MAX], max[MAX][MAX], alloc[MAX][MAX], need[MAX][MAX];
    bool finish[MAX] = {false};
    int work[MAX], safeSeq[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &P);
    printf("Enter number of resources: ");
    scanf("%d", &R);

    printf("\nEnter Available Resources:\n");
    for (int i = 0; i < R; i++)
        scanf("%d", &avail[i]);

    printf("\nEnter Maximum Matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &max[i][j]);
    }

    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("P%d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &alloc[i][j]);
    }

    // Calculate initial work and need
    calculateNeed(need, max, alloc);
    for (int i = 0; i < R; i++)
        work[i] = avail[i];

    printf("\nFinding all possible safe sequences...\n");
    findSafe(avail, max, alloc, finish, work, safeSeq, 0);

    return 0;
}