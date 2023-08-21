#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define M 1000
#define N 1000
#define P 1000
#define NUM_THREADS 4


int A[M][N];
int B[N][P];
int C[M][P];

void multiplyMatrices() {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < P; ++j) {
            for (int k = 0; k < N; ++k) {
                (*(*(C+i)+j)) += (*(*(A+i)+k)) * (*(*(B+k)+j));
            }
        }
    }
    gettimeofday(&end_time, NULL);
    double computation_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_usec - start_time.tv_usec) / 1e6;

    printf("Matrix multiplication using Naive took %.6f seconds\n", computation_time);
}

void *matrix_multiply(void *thread_id) {
    long tid = (long)thread_id;
    int chunk_size = M / NUM_THREADS;
    int start_row = tid * chunk_size;
    int end_row = (tid == NUM_THREADS - 1) ? M : start_row + chunk_size;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < P; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() / (int)RAND_MAX;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            B[i][j] = rand() / (int)RAND_MAX;
        }
    }
    multiplyMatrices();
    
    pthread_t threads[NUM_THREADS];
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, matrix_multiply, (void *)i);
    }

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end_time, NULL);
    double computation_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_usec - start_time.tv_usec) / 1e6;

    printf("Matrix multiplication using %d Threads took %.6f seconds\n", NUM_THREADS,computation_time);

    return 0;
}
