#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_PROCESSES 4
#define MAX_RESOURCES 3

#define DEADLOCK_AVOIDANCE 1
// If we set DEADLOCK_AVOIDANCE to 0, then the program will run without deadlock avoidance

sem_t semaphores[MAX_RESOURCES];
pthread_mutex_t lock;
int available[MAX_RESOURCES];
int MAX[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];


bool isSafe()
{
    int temp[MAX_RESOURCES];
    bool finished[MAX_PROCESSES];

    for (int i = 0; i < MAX_RESOURCES; i++)
        temp[i] = available[i];

    for (int i = 0; i < MAX_PROCESSES; i++)
        finished[i] = 0;

    int count = 0;
    while (count < MAX_PROCESSES)
    {
        bool flag = true;
        for (int i = 0; i < MAX_PROCESSES; i++)
        {
            if (!finished[i])
            {
                int j;
                for (j = 0; j < MAX_RESOURCES; j++)
                {
                    if (need[i][j] > temp[j])
                        break;
                }
                if (j == MAX_RESOURCES)
                {
                    for (int k = 0; k < MAX_RESOURCES; k++)
                        temp[k] += allocation[i][k];
                    finished[i] = true;
                    flag = false;
                    count++;
                }
            }
        }
        if (flag)
            return false;
    }

    return true;
}

void processRequest(int process, int request[])
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (request[i] > available[i])
        {
            printf("Process %d's request is invalid.\n", process + 1);
            return;
        }
        if (request[i] > need[process][i])
        {
            printf("Process %d's request is invalid.\n", process + 1);
            return;
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++)
        for (int j = 0; j < request[i]; j++)
            if (sem_wait(&semaphores[i]) != 0)
            {
                printf("Error in sem_wait for s[%d]", i);
                exit(EXIT_FAILURE);
            }

    // Allocate the resources
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }
    if (DEADLOCK_AVOIDANCE)
    {

        if (isSafe())
        {
            printf("Process %d has been granted resources.\n", process + 1);
        }
        else
        {
            printf("Process %d's has been denied resources.\n", process + 1);

            // Deallocate the resources
            for (int i = 0; i < MAX_RESOURCES; i++)
            {
                available[i] += request[i];
                allocation[process][i] -= request[i];
                need[process][i] += request[i];
            }
        }
    }
    for (int i = 0; i < MAX_RESOURCES; i++)
        for (int j = 0; j < request[i]; j++)
            if (sem_post(&semaphores[i]) != 0)
            {
                printf("Error in sem_post for s[%d]", i);
                exit(EXIT_FAILURE);
            }
}

void freeResources(int process)
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        available[i] += allocation[process][i];
        allocation[process][i] = 0;
        need[process][i] = 0;
    }
}

bool checkNeed(int process)
{
    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (need[process][i] > 0)
            return true;
    }
    return false;
}

void *processThread(void *arg)
{
    int process = *(int *)arg;
    int request[MAX_RESOURCES];

    while (checkNeed(process))
    {
        if (pthread_mutex_lock(&lock))
        {
            perror("Error in pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Process %d has acquired mutex lock with need", process);
        }
        for (int i = 0; i < MAX_RESOURCES; i++)
            request[i] = rand() % (need[process][i] + 1);
        printf("Process %d requested : ", process);
        for (int i = 0; i < MAX_RESOURCES; i++)
            printf("%d ", request[i]);
        printf("\n");
        processRequest(process, request);
        // sleep(1); //
        pthread_mutex_unlock(&lock);
    }
    printf("Process %d has started releasing resources.\n", process + 1);
    // sleep(1);
    printf("Process %d has finished releasing resources.\n", process + 1);

    // acquire lock
    if (pthread_mutex_lock(&lock))
    {
        perror("Error in pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Process %d has acquired mutex lock with need", process);
    }
    freeResources(process);
    if (pthread_mutex_unlock(&lock))
    {
        perror("Error in pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Process %d has released mutex lock with need", process);
    }
    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));
    printf("Enter the number of instances of each resource type : ");
    for (int j = 0; j < MAX_RESOURCES; j++)
    {
        scanf("%d", &available[j]);
    }

    printf("Enter the maximum number of instances of each resource type for each process : ");
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        for (int j = 0; j < MAX_RESOURCES; j++)
            scanf("%d", &allocation[i][j]);
    }

    for (int j = 0; j < MAX_RESOURCES; j++)
    {
        for (int i = 0; i < MAX_PROCESSES; i++)
            need[i][j] = rand() % (available[j] + 1);
    }

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (sem_init(&semaphores[i], 0, available[i]) != 0)
        {
            printf("Error initialising semaphore\n");
            return EXIT_FAILURE;
        }
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("The mutex lock initialization has failed\n");
        return 0;
    }

    pthread_t threads[MAX_PROCESSES];

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        int *temp = (int *)malloc(sizeof(int));
        *temp = i;
        if (pthread_create(&threads[i], NULL, processThread, (void *)temp))
        {
            printf("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            printf("Failed to join thread");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < MAX_RESOURCES; i++)
    {
        if (sem_destroy(&semaphores[i]) != 0)
        {
            printf("Error in sem_destroy for s[%d]", i);
            return EXIT_FAILURE;
        }
    }
}

// 4 6 5 
// 0 0 0 
// 0 0 0 
// 0 0 0 
// 0 0 0 