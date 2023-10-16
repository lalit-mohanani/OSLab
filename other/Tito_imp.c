#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define num_pro 3
#define num_re 4

int avail[num_re];
int max[num_pro][num_re];
int alloc[num_pro][num_re];
int need[num_pro][num_re];

sem_t ps[num_re];
pthread_mutex_t mutex;

bool is_safe()
{
    int ind = 0;
    int work[num_re];
    int f[num_pro];
    int ans[num_pro];

    for (int i = 0; i < num_re; i++)
        work[i] = avail[i];

    for (int i = 0; i < num_pro; i++)
    {
        f[i] = 0;
    }

    for (int k = 0; k < num_pro; k++)
    {
        for (int i = 0; i < num_pro; i++)
        {
            if (f[i] == 0)
            {
                int flag = 0;
                for (int j = 0; j < num_re; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                {
                    ans[ind++] = i;
                    for (int j = 0; j < num_re; j++)
                    {
                        work[j] = work[j] + alloc[i][j];
                    }
                    f[i] = 1;
                }
            }
        }
    }
    bool safe = true;
    for (int i = 0; i < num_pro; i++)
    {
        if (f[i] == 0)
        {
            safe = false;
            break;
        }
    }
    // if (safe)
    // {
    //     printf("The safe sequence is: ");
    //     for (int i = 0; i < num_pro; i++)
    //     {
    //         printf("%d ", ans[i]);
    //     }
    //     printf("\n");
    // }
    return safe;
}

void *process(void *arg)
{
    int id = *(int *)arg;
    srand(time(NULL));
    while (1)
    {
        bool no_need = true;
        for (int i = 0; i < num_re; i++)
        {
            if (need[id - 1][i])
            {
                no_need = false;
                break;
            }
        }

        if (no_need)
            break;

        pthread_mutex_lock(&mutex);
        int request[num_re];
        printf("Process %d requested: ", id);
        for (int i = 0; i < num_re; i++)
        {
            request[i] = rand() % (need[id - 1][i] + 1);
            printf("%d ", request[i]);
        }
        printf("\n");
        int flag = 0;
        for (int i = 0; i < num_re; i++)
        {
            if (request[i] > need[id - 1][i])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 1)
        {
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            for (int i = 0; i < num_re; i++)
            {
                avail[i] = avail[i] - request[i];
                need[id - 1][i] = need[id - 1][i] - request[i];
                alloc[id - 1][i] = alloc[id - 1][i] + request[i];
            }

            if (is_safe())
            {
                printf("Request for process %d is accepted: \n", id);
                for (int i = 0; i < num_re; i++)
                {
                    while (request[i]--)
                        sem_wait(&ps[i]);
                }
            }
            else
            {
                printf("Request for process %d is not accepted: \n", id);
                for (int i = 0; i < num_re; i++)
                {
                    avail[i] = avail[i] + request[i];
                    need[id - 1][i] = need[id - 1][i] + request[i];
                    alloc[id - 1][i] = alloc[id - 1][i] - request[i];
                }
            }

            pthread_mutex_unlock(&mutex);
            sleep(1);
        }
    }
    printf("............Process %d started............\n", id);
    sleep(2);
    printf("............Process %d ended............\n", id);

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_re; i++)
    {
        avail[i] = avail[i] + alloc[id - 1][i];
        while (alloc[id - 1][i]--)
            sem_post(&ps[i]);
    }
    pthread_mutex_unlock(&mutex);
    free(arg);
    return NULL;
}

int main()
{
    pthread_t th[num_pro];

    printf("Enter the number of available instances of resource:\n");
    for (int i = 0; i < num_re; i++)
    {
        scanf("%d", &avail[i]);
        sem_init(&ps[i], 0, avail[i]);
    }
    printf("Enter the MAXIMUM demand of each resource for each process:\n");
    for (int i = 0; i < num_pro; i++)
    {
        for (int j = 0; j < num_re; j++)
        {
            scanf("%d", &max[i][j]);
            alloc[i][j] = 0;
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    for (int i = 0; i < num_pro; i++)
    {
        int *p = (int *)malloc(sizeof(int));
        *p = i + 1;
        pthread_create(&th[i], NULL, &process, (void *)p);
    }

    for (int i = 0; i < num_pro; i++)
    {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < num_re; i++)
        sem_destroy(&ps[i]);

    return 0;
}

/*
6 8 4 5
2 3 3 1
4 3 2 3
4 4 1 2
*/