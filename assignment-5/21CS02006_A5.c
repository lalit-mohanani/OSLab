// Roll no.: 21CS02006
// Name: Lalit Mohanani
// Assignment 5
// 1. Implement Floyd Warshall Algorithm using pthreads and semaphores.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define MAX 1001
#define INF (int)1e9 + 7

// adacency matrix for graph
// dist matrix for storing the shortest distance between any 2 nodes
int adacency[MAX][MAX];
int dist[MAX][MAX];

// n,m for no. of nodes and edges
int n, m;

// semaphore for read and write
sem_t wr;
// mutex for read count
pthread_mutex_t mutex;
int readerCounter; //count of readers

void *Floyd(void *arg){ // function for loop of i in floyd warshall algorithm
    int *arr = (int *)arg;
    int k = arr[0], i = arr[1]; // k for kth iteration, i for ith node

    for (int j = 1; j <= n; j++){
        int flag = 0;
        pthread_mutex_lock(&mutex); // lock for reader counter
        readerCounter++;
        if (readerCounter == 1) // for first reader lock the writer
            sem_wait(&wr);
        pthread_mutex_unlock(&mutex);

        // reading is done by each thread concurrently
        if (dist[i][k] + dist[k][j] < dist[i][j]){ 
            flag = 1;
        }

        pthread_mutex_lock(&mutex);
        readerCounter--;
        if (readerCounter == 0) // exit condition for last reader
            sem_post(&wr); // unlock the writer
        pthread_mutex_unlock(&mutex);

        sem_wait(&wr); // lock the writer
        if (flag == 1){ // final writing is done by only one thread at a time
            dist[i][j] = dist[i][k] + dist[k][j];
        }
        sem_post(&wr);
    }
    return NULL;
}

int main(){
    pthread_mutex_init(&mutex, NULL);
    int x, y;
    printf("Enter the no. of nodes and the no. of edges\n");
    scanf("%d %d", &x, &y);
    n = x; m = y;

    memset(adacency, -1, sizeof(adacency));
    memset(dist, INF, sizeof(dist));

    printf("Enter the 2 nodes for each edge along with it's weight\n");

    for (int i = 0; i < m; i++){
        int x,y,z;
        scanf("%d %d %d", &x, &y, &z);
        adacency[x][y] = z;
        adacency[y][x] = z;
        dist[x][y] = z;
        dist[y][x] = z;
    }
    
    for (int i = 0; i <= n; i++){
        dist[i][i] = 0;
    }

    for (int k = 1; k <= n; k++){
        pthread_t th[n];
        sem_init(&wr, 0, 1);
        for (int i = 0; i < n; i++){
            int *p = (int *)malloc(2 * sizeof(int));
            p[0] = k;
            p[1] = i + 1;
            pthread_create(&th[i], NULL, &Floyd, (void *)p);
        }

        for (int i = 0; i < n; i++){
            pthread_join(th[i], NULL);
        }
    }

    printf("The shortest distance between any 2 nodes is\n");
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            if (dist[i][j] == 1e9 + 7)
                printf("%d ", -1);
            else
                printf("%d ", dist[i][j]);
        }
        printf("\n");
    }
    sem_destroy(&wr);
    pthread_mutex_destroy(&mutex);
    return 0;
}