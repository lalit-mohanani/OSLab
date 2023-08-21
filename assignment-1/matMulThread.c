#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

const size_t M = 3002;
const size_t K = 10000;
const size_t N = 1000;

const int MAX_VAL = 100;

const int NUM_THREADS = 16;

long long int CHUNK_SIZE;
long long int MAX_CELL;

int **a, **b;
long long int **c;

const bool show_output = false;

void printConfiguration(){
    printf("Printing Configuration:\n");
    printf("M = %ld\n", M);
    printf("K = %ld\n", K);
    printf("N = %ld\n", N);
    printf("MAX_VAL = %d\n", MAX_VAL);
    printf("NUM_THREADS = %d\n", NUM_THREADS);
    printf("CHUNK_SIZE = %lld\n", CHUNK_SIZE);
    printf("MAX_CELL = %lld\n\n", MAX_CELL);
}

void* matMulParallel(void* arg)
{
    int curr_cell = *((int *) arg);
    int row, col, i, k;
    
    for(i=curr_cell;i<((curr_cell+CHUNK_SIZE)>MAX_CELL?MAX_CELL:(curr_cell+CHUNK_SIZE));i++){
        row = i / N;
        col = i % N;
        c[row][col] = 0;
        for(k=0;k<K;k++){
            c[row][col] += a[row][k]*b[k][col];
        }
    }

    return NULL;
}

void verify_result() {
    
    struct timespec start_time, end_time;
    printf("\nCPU verification has started\n");
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    for (int row = 0; row < M; row++) {
        for (int col = 0; col < N; col++) {
            long long tmp = 0;
            for (int i = 0; i < K; i++) {
                tmp += a[row][i] * b[i][col];
            }
            if (tmp != c[row][col]) {
                printf("Verification failed! (row=%d, col=%d, true_val=%lld, c=%lld)\n", row, col, tmp, c[row][col]);
                exit(-1);
            }
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    printf("Result Verified by CPU!\n");
    double elapsed = (end_time.tv_sec - start_time.tv_sec);
    elapsed += (end_time.tv_nsec - start_time.tv_nsec) / 1000000000.0;
    printf("CPU verification took %lf ms\n", elapsed*1000);
}


int main()
{
    int i,j;
    MAX_CELL = M*N;
    CHUNK_SIZE = ceil(MAX_CELL/(double)NUM_THREADS);
    long long int curr_cell = 0;

    printConfiguration();

    a = (int **)malloc(M * sizeof(int *));
    b = (int **)malloc(K * sizeof(int *));
    c = (long long int **)malloc(M * sizeof(long long int *));

    srand(time(NULL));

    printf("Populating matrices with random values\n");

    for(i=0;i<M;i++){
        a[i] = (int *)malloc(K * sizeof(int));
        for(j=0;j<K;j++)
            a[i][j] = rand()%MAX_VAL;
        
    }

    for(i=0;i<K;i++){
        b[i] = (int *)malloc(N * sizeof(int));
        for(j=0;j<N;j++)
            b[i][j] = rand()%MAX_VAL;
    }

    for(i=0;i<M;i++){
        c[i] = (long long int *)malloc(N * sizeof(long long int));
    }

    printf("Populating matrices done\n");

    printf("\nMulti-threaded calculation started\n");
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
 
    for(i=0;i<NUM_THREADS;i++){
        thread_args[i] = curr_cell;
        pthread_create(&threads[i], NULL, matMulParallel, (void *) &thread_args[i]);
        curr_cell += CHUNK_SIZE;
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("\nMulti-threaded calculation took %lf ms\n", elapsed*1000);

    if(show_output){
        printf("\nMatrix A:\n");
        for(i=0;i<M;i++){
            for(j=0;j<K;j++)
                printf("%d ", a[i][j]);
            
            printf("\n");
        }

        printf("\nMatrix B:\n");
        for(i=0;i<K;i++){
            for(j=0;j<N;j++)
                printf("%d ", b[i][j]);
            
            printf("\n");
        }

        printf("\nMatrix C:\n");
        for(i=0;i<M;i++){
            for(j=0;j<N;j++)
                printf("%lld ", c[i][j]);
            
            printf("\n");
        }
    }

    verify_result();

    return 0;
}