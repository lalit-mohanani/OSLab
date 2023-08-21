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


void *SUM(void *thread_id) {
    long tid = (long)thread_id;
    long long int sum = 0;
    for(int i = 0+tid; i < N; i+=THREADS) {
        sum += arr[i];
    }
    long long *p = (long long *)malloc(sizeof(long long));
	*p = sum;
	pthread_exit(p);
}

int main() {
    for (int i = 0; i < N; i++) {
        arr[i] = rand()%(int)RANDMAX;
    }
    long long ans = 0;
    printf("Enter no of threads \n");
    scanf("%d", &THREADS);

    pthread_t threads[THREADS];
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for (long i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, SUM, (void *)i);
    }

    for (long i = 0; i < THREADS; i++) {
        void *k;
	    pthread_join(threads[i], &k);
		long long  *p = (long long *)k;
        ans += *p;
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