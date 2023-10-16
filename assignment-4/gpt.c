#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_RESOURCES 4
#define NUM_PROCESSES 3

int available[NUM_RESOURCES];
int maximum[NUM_PROCESSES][NUM_RESOURCES];
int allocation[NUM_PROCESSES][NUM_RESOURCES];
int need[NUM_PROCESSES][NUM_RESOURCES];

sem_t s[NUM_RESOURCES];

void initialize() {
    // Initialize available resources
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("Enter the number of available instances of resource %d: ", i);
        scanf("%d", &available[i]);
        sem_init(&s[i], 0, available[i]);
    }

    // Initialize maximum demand matrix
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Enter the maximum demand of process %d: ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }
}

void request_resources(int process, int request[]) {
    // Check if the request is valid
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > need[process][i] || request[i] > available[i]) {
            printf("Process %d's request is invalid.\n", process);
            return;
        }
    }

    // Try to allocate resources
    if (sem_trywait(&s[0]) == 0 &&
        sem_trywait(&s[1]) == 0 &&
        sem_trywait(&s[2]) == 0 &&
        sem_trywait(&s[3]) == 0) {
        
        for (int i = 0; i < NUM_RESOURCES; i++) {
            available[i] -= request[i];
            allocation[process][i] += request[i];
            need[process][i] -= request[i];
        }

        if (is_safe_state()) {
            printf("Process %d has been granted resources.\n", process);
        } else {
            printf("Process %d's request could not be granted to maintain safety.\n", process);

            // Roll back the resource allocation
            for (int i = 0; i < NUM_RESOURCES; i++) {
                available[i] += request[i];
                allocation[process][i] -= request[i];
                need[process][i] += request[i];
            }
        }
    } else {
        printf("Process %d's request is waiting.\n", process);
    }
}

void release_resources(int process) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] += allocation[process][i];
        allocation[process][i] = 0;
        need[process][i] = maximum[process][i];
        sem_post(&s[i]);
    }
}

int is_safe_state() {
    int work[NUM_RESOURCES];
    int finish[NUM_PROCESSES];

    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i];
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        finish[i] = 0;
    }

    int count = 0;
    while (count < NUM_PROCESSES) {
        int found = 0;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < NUM_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == NUM_RESOURCES) {
                    for (int k = 0; k < NUM_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if (!found) {
            return 0;  // Unsafe state
        }
    }

    return 1;  // Safe state
}

void* process_thread(void* arg) {
    int process = *(int*)arg;
    int request[NUM_RESOURCES];

    // Simulate the process making requests
    for (int i = 0; i < NUM_RESOURCES; i++) {
        request[i] = rand() % (maximum[process][i] + 1);
    }

    request_resources(process, request);
    sleep(1); // Simulate the process doing work
    release_resources(process);

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    initialize();

    pthread_t threads[NUM_PROCESSES];
    int thread_args[NUM_PROCESSES];

    // Create threads for each process
    for (int i = 0; i < NUM_PROCESSES; i++) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, process_thread, &thread_args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All processes have finished.\n");

    return 0;
}
