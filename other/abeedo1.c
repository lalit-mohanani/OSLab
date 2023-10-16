#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUMEBER_OF_RESOURCES 4
#define NUMEBER_OF_PROCESSES 3
sem_t R[NUMEBER_OF_RESOURCES];
int Available[NUMEBER_OF_RESOURCES];
int Need[NUMEBER_OF_PROCESSES][NUMEBER_OF_RESOURCES];
int Allocated[NUMEBER_OF_PROCESSES][NUMEBER_OF_RESOURCES];
pthread_mutex_t mutex;

// returns 0 if safe else returns -1
int is_safe()
{
    int finished[NUMEBER_OF_PROCESSES] = {0};
    int work[NUMEBER_OF_RESOURCES];
    for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        work[i] = Available[i];
    while (1)
    {
        int found = 0;
        for (int i = 0; i < NUMEBER_OF_PROCESSES; i++)
        {
            if (finished[i] == 0)
            {
                int are_available = 1;
                for (int j = 0; j < NUMEBER_OF_RESOURCES; j++)
                {
                    if (Need[i][j] > work[j])
                        are_available = 0;
                }
                if (are_available)
                {
                    found = 1;
                    for (int j = 0; j < NUMEBER_OF_RESOURCES; j++)
                    {
                        work[j] += Allocated[i][j];
                    }
                    finished[i] = 1;
                }
            }
        }
        if (!found)
        {
            for (int i = 0; i < NUMEBER_OF_PROCESSES; i++)
                if (!finished[i])
                    return -1;
            return 0;
        }
    }
}

void *process_thread(void *args)
{
    int pid = *(int *)args;
    srand(time(NULL));
    while (1)
    {
        int all_available = 1;
        for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        {
            if (Need[pid - 1][i])
            {
                all_available = 0;
                break;
            }
        }
        // if all resources are available then process starts after breaking out of the loop
        if (all_available)
            break;

        pthread_mutex_lock(&mutex);
        // random generation of request
        int request[NUMEBER_OF_RESOURCES];
        printf("Process %d requested : ", pid);
        for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        {
            request[i] = (rand() % (Need[pid - 1][i] + 1));
            printf("%d ", request[i]);
        }
        printf("\n");
        int flag = 1;
        for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        {
            if (request[i] > Need[pid - 1][i])
            {
                printf("Invalid Request by process %d\n", pid);
                flag = 0;
                break;
            }
        }
        /* if the process is invalid unlock the mutex and continue and in this there will be no
           invalid requests becuause of modular with (needi+1)
        */
        if (!flag)
        {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        {
            Available[i] -= request[i];
            Need[pid - 1][i] -= request[i];
            Allocated[pid - 1][i] += request[i];
        }
        // checks whether request is safe
        if (is_safe() == 0)
        {
            // request is accepted and the resources(semaphores) are allocated
            printf("Request from process %d is accepted\n", pid);
            for (int j = 0; j < NUMEBER_OF_RESOURCES; j++)
            {
                while (request[j]--)
                    sem_wait(&R[j]);
            }
        }
        else
        {
            // request is not safe so denied
            printf("Request denied from the process %d\n", pid);
            for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
            {
                Available[i] += request[i];
                Need[pid - 1][i] += request[i];
                Allocated[pid - 1][i] -= request[i];
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    // All resources are availabe to start the process
    printf("******* Processs %d started *******\n", pid);
    // Process Execution
    sleep(5);

    printf("******* Process %d completed *******\n", pid);
    // resources are released
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
    {
        Available[i] += Allocated[pid - 1][i];
        while (Allocated[pid - 1][i]--)
            sem_post(&R[i]);
    }
    pthread_mutex_unlock(&mutex);
    free(args);
    return NULL;
}

int main()
{
    printf("Enter available resources: ");
    for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
    {
        scanf("%d", &Available[i]);
        sem_init(&R[i], 0, Available[i]);
    }
    pthread_mutex_init(&mutex, 0);

    printf("Enter Need matrix :");
    for (int i = 0; i < NUMEBER_OF_PROCESSES; i++)
    {
        for (int j = 0; j < NUMEBER_OF_RESOURCES; j++)
        {
            scanf("%d", &Need[i][j]); // no resources were allocated initially so need is equal to MAX
            Allocated[i][j] = 0;      // no resources are allocated initailly
        }
    }

    pthread_t threads[NUMEBER_OF_PROCESSES];
    for (int i = 0; i < NUMEBER_OF_PROCESSES; i++)
    {
        int *pid;
        pid = (int *)malloc(sizeof(int));
        *pid = i + 1;
        pthread_create(&threads[i], NULL, process_thread, (void *)pid);
    }

    for (int i = 0; i < NUMEBER_OF_PROCESSES; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUMEBER_OF_RESOURCES; i++)
        sem_destroy(&R[i]);
    return 0;
}

/*
    input format:
        available resources:    6 8 4 5
        Need Matrix :   2 3 3 1
                        4 3 2 3
                        4 4 1 2
*/

/*
    part-2
    Directly allocate the resources without calling the is_safe function, then for the same input
    provided above there will be a deadlock.
*/