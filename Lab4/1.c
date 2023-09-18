// Lalit Mohanani (21CS02006)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
// for max value of element in array
#define RANDMAX 100
// no of elements in array
#define N (int)1e6
// Default no of Threads
int THREADS = 1;
int arr[N];
long long int ans = 0; 
pthread_mutex_t lock;

void *SUM(void *thread_id) {
    long tid = (long)thread_id;
    long long int sum = 0;
    for(int i = 0+tid; i < N; i+=THREADS) {
        sum += arr[i];
    }
    pthread_mutex_lock(&lock);
        ans += sum;
    pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < N; i++) {
        arr[i] = rand()%(int)RANDMAX;
    }
    printf("Enter no of threads \n");
    scanf("%d", &THREADS);

    pthread_t threads[THREADS];
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return 1;
	}
    for (long i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, SUM, (void *)i) != 0){
            printf("\ncreate threads failed\n");
            return 1;
        }
    }

    for (long i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0){
            printf("\n threads join failed\n");
            return 1;
        }
    }

    if (pthread_mutex_destroy(&lock) != 0) {
		printf("\n mutex destroy has failed\n");
		return 1;
	}

    gettimeofday(&end_time, NULL);
    // for (int i = 0; i < N; i++) {
    //     printf("%d " , arr[i]);
    // }
    printf("%lld \n",ans);
    double computation_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
    printf("Vector Sum using %d Threads took %.6f seconds\n", THREADS,computation_time);

    return 0;
}
