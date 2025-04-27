#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct {
    int id, at, bt, remaining_bt, completion_time, pr;
    float wt, tat;
} Process;

Process p[MAX], p1[MAX];
int gantt_chart[100];

void turnwait(int n) {
    float total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].completion_time - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
    }
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%.2f\t%.2f\n", p[i].id, p[i].at, p[i].bt, p[i].completion_time, p[i].tat, p[i].wt);
    }
    printf("\nAverage Turn Around Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int accept(int ch) {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("AT of P%d: ", i + 1);
        scanf("%d", &p[i].at);
    }
    for (int i = 0; i < n; i++) {
        printf("BT of P%d: ", i + 1);
        scanf("%d", &p[i].bt);
        p[i].remaining_bt = p[i].bt;
    }
    if (ch == 3 || ch == 4) {
        for (int i = 0; i < n; i++) {
            printf("Priority of P%d: ", i + 1);
            scanf("%d", &p[i].pr);
        }
    }
    for (int i = 0; i < n; i++) {
        p1[i] = p[i]; // For RR
    }
    return n;
}

void fcfs(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                Process temp = p[i]; 
                p[i] = p[j]; 
                p[j] = temp;
            }
        }
    }
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) 
          time = p[i].at;
        p[i].completion_time = time + p[i].bt;
        time = p[i].completion_time;
    }
    printf("\nGantt Chart (FCFS):\n");
    for (int i = 0; i < n; i++) printf("P%d ", p[i].id);
    printf("\n");
}

void sjf_non_preemptive(int n) {
    int completed = 0, time = 0;
    int done[MAX] = {0};
    while (completed < n) {
        int idx = -1, min_bt = 1e9;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if (idx != -1) {
            time += p[idx].bt;
            p[idx].completion_time = time;
            done[idx] = 1;
            completed++;
        } else {
            time++;
        }
    }
    printf("\nGantt Chart (SJF Non-Preemptive):\n");
    for (int i = 0; i < n; i++) printf("P%d ", p[i].id);
    printf("\n");
}

void sjf_preemptive(int n) {
    int completed = 0, time = 0;
    int done[MAX] = {0};
    while (completed < n) {
        int idx = -1, min_bt = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0 && p[i].remaining_bt < min_bt) {
                min_bt = p[i].remaining_bt;
                idx = i;
            }
        }
        if (idx != -1) {
            p[idx].remaining_bt--;
            gantt_chart[time] = p[idx].id;
            if (p[idx].remaining_bt == 0) {
                p[idx].completion_time = time + 1;
                completed++;
            }
        } else {
            gantt_chart[time] = 0; // idle
        }
        time++;
    }
    printf("\nGantt Chart (SJF Preemptive):\n");
    for (int i = 0; i < time; i++) {
        if (gantt_chart[i] == 0)
            printf("Idle ");
        else
            printf("P%d ", gantt_chart[i]);
    }
    printf("\n");
}

void priority_non_preemptive(int n) {
    int completed = 0, time = 0;
    int done[MAX] = {0};
    while (completed < n) {
        int idx = -1, high = 1e9;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].pr < high) {
                high = p[i].pr;
                idx = i;
            }
        }
        if (idx != -1) {
            time += p[idx].bt;
            p[idx].completion_time = time;
            done[idx] = 1;
            completed++;
        } else {
            time++;
        }
    }
    printf("\nGantt Chart (Non-Preemptive Priority):\n");
    for (int i = 0; i < n; i++) printf("P%d ", p[i].id);
    printf("\n");
}

void priority_preemptive(int n) {
    int completed = 0, time = 0;
    int idx = -1;
    while (completed < n) {
        int high = 1e9;
        idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining_bt > 0 && p[i].pr < high) {
                high = p[i].pr;
                idx = i;
            }
        }
        if (idx != -1) {
            p[idx].remaining_bt--;
            gantt_chart[time] = p[idx].id;
            if (p[idx].remaining_bt == 0) {
                p[idx].completion_time = time + 1;
                completed++;
            }
        } else {
            gantt_chart[time] = 0; // idle
        }
        time++;
    }
    printf("\nGantt Chart (Preemptive Priority):\n");
    for (int i = 0; i < time; i++) {
        if (gantt_chart[i] == 0)
            printf("Idle ");
        else
            printf("P%d ", gantt_chart[i]);
    }
    printf("\n");
}

void round_robin(int n) {
    int time, tq, front = 0, rear = -1, q[100], visited[MAX] = {0}, total = 0;
    printf("Enter Time Quantum: ");
    scanf("%d", &tq);
    for (time = 0; total < n;) {
        for (int i = 0; i < n; i++) {
            if (p1[i].at <= time && !visited[i]) {
                q[++rear] = i;
                visited[i] = 1;
            }
        }
        if (front > rear) {
            time++;
            continue;
        }
        int idx = q[front++];
        if (p1[idx].bt > tq) {
            time += tq;
            p1[idx].bt -= tq;
        } else {
            time += p1[idx].bt;
            p1[idx].bt = 0;
            p[idx].completion_time = time;
            total++;
        }
        for (int i = 0; i < n; i++) {
            if (p1[i].at <= time && !visited[i]) {
                q[++rear] = i;
                visited[i] = 1;
            }
        }
        if (p1[idx].bt > 0)
            q[++rear] = idx;
    }
    printf("\nGantt Chart (Round Robin):\n");
    for (int i = 0; i <= rear; i++) printf("P%d ", p[q[i]].id);
    printf("\n");
}

int main() {
    int n, ch;
    printf("Choose Scheduling Algorithm:\n");
    printf("1. FCFS\n2. SJF (Non-Preemptive)\n3. Preemptive Priority\n4. Non-Preemptive Priority\n5. Round Robin\n6. SJF (Preemptive)\n");
    scanf("%d", &ch);
    n = accept(ch);

    switch (ch) {
        case 1: fcfs(n); break;
        case 2: sjf_non_preemptive(n); break;
        case 3: priority_preemptive(n); break;
        case 4: priority_non_preemptive(n); break;
        case 5: round_robin(n); break;
        case 6: sjf_preemptive(n); break;
        default: printf("Invalid Choice!\n"); return 0;
    }

    turnwait(n);
    return 0;
}
