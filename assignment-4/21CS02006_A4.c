// Assignment - 4
// Deadline -11/10/23
// Create 3 process(Threads)(t1,t2,t3)
// Create 4 counting semaphore(s1,s2,s3,s4)
// Declare the need matrix before hand (ensure that need <= available at the begining for all process)
// randomly generate request for each process.
// i) Follow the Banker's algorithm to allocate the resourse(semaphore) and show an exit path for the system where each process is 
// finished and no deadlock happens.
// ii) Also show counter example to create deadlock if the Banker's Algorithm is not followed.

// Lalit Mohanani (21cs02006)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_RESOURCES 4
#define NUM_PROCESSES 3

bool use_bankers = false; // use this variable to switch between bankers algorithm and normal algorithm

int available[NUM_RESOURCES];
int maximum[NUM_PROCESSES][NUM_RESOURCES];
int allocation[NUM_PROCESSES][NUM_RESOURCES];
int need[NUM_PROCESSES][NUM_RESOURCES];

sem_t s[NUM_RESOURCES];
pthread_mutex_t mutex;

void sem_increase(sem_t *__sem,int val){
    int i;
    for(i=0;i<val;i++){
        sem_post(__sem);
    }
    // sem_getvalue(__sem, &i);
    // printf("sem value: %d\n", i);

}
void sem_decrease(sem_t *__sem,int val){
    int i;
    for(i=0;i<val;i++){
        sem_wait(__sem);
    }
    // sem_getvalue(__sem, &i);
    // printf("sem value: %d\n", i);
}


void initialize() {
    // Initialize available resources
    pthread_mutex_init(&mutex,NULL);
    memset(allocation,0,sizeof(int)*NUM_RESOURCES*NUM_PROCESSES);

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
            need[i][j] = maximum[i][j];
        }
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
    // for (int i = 0; i < NUM_RESOURCES; i++) {
    //     request[i] = rand() % (maximum[process][i] + 1);
    // }

    // request_resources(process, request);
    // sleep(1); // Simulate the process doing work
    // release_resources(process);
    // printf("Process %d has finished.\n", process);

    // int ct =0;
    while (1){
        int flag = 1;
        for (int i = 0; i < NUM_RESOURCES; i++){
            if (need[process][i]){
                flag = 0;
                break;
            }
        }
        // if all resources are available then process starts after breaking out of the loop
        if (flag)
            break;

        pthread_mutex_lock(&mutex);
        // random generation of request
        int request[NUM_RESOURCES];
        printf("Process %d requested : ", process);
        for (int i = 0; i < NUM_RESOURCES; i++){
            // if(!ct)
                request[i] = (rand() % (need[process][i] + 1));
            // else
                // request[i] = need[process][i];
            printf("%d ", request[i]);
        }
        printf("\n");
       
        // for (int i = 0; i < NUM_RESOURCES; i++){
        //     if (request[i] > need[process - 1][i]){
        //         printf("Invalid Request by process %d\n", process);
        //         flag = 0;
        //         break;
        //     }
        // }
        /* if the process is invalid unlock the mutex and continue and in this there will be no
           invalid requests becuause of modular with (needi+1)
        */
        // if (!flag)
        // {
        //     pthread_mutex_unlock(&mutex);
        //     continue;
        // }
        for (int i = 0; i < NUM_RESOURCES; i++){
            available[i] -= request[i];
            need[process][i] -= request[i];
            allocation[process][i] += request[i];
        }
        // checks whether request is safe
        if (is_safe_state() || use_bankers){
            // ct = 1;
            // request is accepted and the resources(semaphores) are allocated
            printf("Request from process %d is accepted\n", process);
            for (int i = 0; i < NUM_RESOURCES; i++){
                sem_decrease(&s[i], request[i]);
            }
        }
        else{
            // request is not safe so denied
            printf("Request denied from the process %d\n", process);
            for (int i = 0; i < NUM_RESOURCES; i++){
                available[i] += request[i];
                need[process][i] += request[i];
                allocation[process][i] -= request[i];
            }
        }
        pthread_mutex_unlock(&mutex);
        // sleep(1);
    }
    // All resources are availabe to start the process
    printf("******* Processs %d started *******\n", process);
    // Process Execution
    // sleep(3);

    printf("******* Process %d completed *******\n", process);
    // resources are released
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < NUM_RESOURCES; i++){
        available[i] += allocation[process][i];
        sem_increase(&s[i], allocation[process][i]);
    }
    pthread_mutex_unlock(&mutex);
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

    pthread_mutex_destroy(&mutex);
    printf("All processes have finished.\n");

    return 0;
}

// Part2: 
// 1. To allocate without safety algorithm make use_bankers = true in line 28
// 2. In this case deadlock may occur

// 10 5 5 5 
// 3 2 3 2 
// 10 3 2 2
// 0 1 5 2

// 6 8 4 5
// 2 3 3 1
// 4 3 2 3
// 4 4 1 2