/*
4 4
1 2 1
2 3 1
3 4 1
4 1 1
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <limits.h>
#include <string.h>

int adj[101][101];
int dist[101][101];
int n, m;
sem_t wr, mutex;
int rc = 0;

void *func(void *arg)
{
    int *arr = (int *)arg;
    int k = arr[0], i = arr[1];

    for (int j = 1; j <= n; j++)
    {
        int f = 0;
        sem_wait(&mutex);
        rc++;
        if (rc == 1)
            sem_wait(&wr);
        sem_post(&mutex);

        if (dist[i][k] + dist[k][j] < dist[i][j])
        {
            f = 1;
        }

        sem_wait(&mutex);
        rc--;
        if (rc == 0)
            sem_post(&wr);
        sem_post(&mutex);

        sem_wait(&wr);
        if (f == 1)
        {
            dist[i][j] = dist[i][k] + dist[k][j];
        }
        sem_post(&wr);
    }

    // for (int j = 1; j <= n; j++)
    // {

    // }
    return NULL;
}

int main()
{
    int a, b;
    printf("Enter the no. of nodes and the no. of edges\n");
    scanf("%d%d", &a, &b);
    n = a;
    m = b;
    memset(adj, (int)1e9 + 7, sizeof(adj));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            dist[i][j] = 1e9 + 7;
    printf("Enter the 2 nodes for each edge along with it's weight\n");

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        adj[u][v] = w;
        adj[v][u] = w;
        dist[u][v] = w;
        dist[v][u] = w;
    }
    for (int i = 0; i <= n; i++)
    {
        dist[i][i] = 0;
    }

    for (int k = 1; k <= n; k++)
    {
        pthread_t th[n];
        sem_init(&mutex, 0, 1);
        sem_init(&wr, 0, 1);
        for (int i = 0; i < n; i++)
        {
            int *p = (int *)malloc(2 * sizeof(int));
            p[0] = k;
            p[1] = i + 1;
            pthread_create(&th[i], NULL, &func, (void *)p);
        }

        for (int i = 0; i < n; i++)
        {
            pthread_join(th[i], NULL);
        }
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (dist[i][j] == 1e9 + 7)
                printf("%d ", -1);
            else
                printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
    return 0;
}