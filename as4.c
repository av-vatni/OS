#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define READERS 3
#define WRITERS 2

int balance = 1000;
int reader_count = 0;

// Synchronization primitives
pthread_mutex_t mutex;
sem_t write_lock;

// Synchronized Reader
void* sync_reader(void* arg){
    int id = *(int*)arg;
    free(arg);
    for(int i=0; i<2; i++){
        pthread_mutex_lock(&mutex);
        reader_count++;
        if(reader_count == 1){
            sem_wait(&write_lock); // Block writers
        }
        pthread_mutex_unlock(&mutex);

        printf("[SYNC Reader-%d] Reads Balance: $%d\n", id, balance);
        usleep(100000);

        pthread_mutex_lock(&mutex);
        reader_count--;
        if(reader_count == 0){
            sem_post(&write_lock); // Allow writers
        }
        pthread_mutex_unlock(&mutex);
        usleep(200000);
    }
    return NULL;
}

// Synchronized Writer
void* sync_writer(void* arg){
    int id = *(int *)arg;
    free(arg);
    for(int i=0; i<2; i++){
        sem_wait(&write_lock); // Lock writer access
        balance += 100;
        printf("[SYNC Writer-%d] Writes New Balance: $%d\n", id, balance);
        sem_post(&write_lock);
        usleep(300000);
    }
    return NULL;
}

// Unsynchronized Reader
void* unsync_reader(void* arg){
    int id = *(int *) arg;
    free(arg);

    for(int i=0; i<2; i++){
        printf("[UNSYNC Reader-%d] Reads Balance: $%d\n", id, balance);
        usleep(100000);
    }
    return NULL;
}

// Unsynchronized Writer
void* unsync_writer(void* arg){
    int id = *(int *)arg;
    free(arg);

    for(int i=0; i<2; i++){
        balance += 100;
        printf("[UNSYNC Writer-%d] Writes Balance: $%d\n", id, balance);
        usleep(300000);
    }
    return NULL;
}

// Run Synchronized
void run_sync(){
    pthread_t r[READERS], w[WRITERS];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&write_lock, 0, 1);

    for (int i = 0; i < READERS; i++){
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(&r[i], NULL, sync_reader, id);
    }
    for(int i = 0; i < WRITERS; i++){
        int* id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(&w[i], NULL, sync_writer, id);
    }
    for(int i = 0; i < READERS; i++){
        pthread_join(r[i], NULL);
    }
    for(int i = 0; i < WRITERS; i++){
        pthread_join(w[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&write_lock);
}

// Run Unsynchronized
void run_async(){
    pthread_t r[READERS], w[WRITERS];
    for (int i = 0; i < READERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&r[i], NULL, unsync_reader, id);
    }

    for (int i = 0; i < WRITERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&w[i], NULL, unsync_writer, id);
    }

    for (int i = 0; i < READERS; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < WRITERS; i++) pthread_join(w[i], NULL);
}

int main() {
    int choice;
    printf("Choose Mode:\n1. Synchronized\n2. Unsynchronized\nEnter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\n--- Synchronized Mode ---\n");
            run_sync();
            break;
        case 2:
            printf("\n--- Unsynchronized Mode ---\n");
            run_async();
            break;
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}
