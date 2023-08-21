#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <sys/time.h>
// #define MAX 1024

int M=1024;
int N=1024;
int P=1024;

void *mult(void* arg){
	int *data = (int *)arg;
	int k = 0, i = 0;
	
	int x = data[0];
	for (i = 1; i <= x; i++)
		k += data[i]*data[i+x];
	
	int *p = (int*)malloc(sizeof(int));
		*p = k;

	pthread_exit(p);
}

void multiplyMatrices(int MatA[][N], int MatB[][P], int MatC[][P]) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < P; ++j) {
            for (int k = 0; k < N; ++k) {
                (*(*(MatC+i)+j)) += (*(*(MatA+i)+k)) * (*(*(MatB+k)+j));
            }
        }
    }
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("%ds %dms\n",seconds, micros);
    // for (int i = 0; i < M; i++){
	// 	for(int j = 0; j < P; j++)
	// 		printf("%d ",MatC[i][j]);
	// 	printf("\n");
	// }
}

void multiplyMatricesMultiThreads(int MatA[][N], int MatB[][P], int MatC[][P]) {
    struct timeval start, end;
    pthread_t *threads;
	threads = (pthread_t*)malloc(M*P*sizeof(pthread_t));
    gettimeofday(&start, NULL);
    int count = 0;
	int* data = NULL;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < P; j++){
			data = (int *)malloc((N+N+1)*sizeof(int));
			data[0] = N;
			for (int k = 0; k < N; k++)
				data[k+1] = MatA[i][k];
	
			for (int k = 0; k < N; k++)
				data[k+N+1] = MatB[k][j];
	        pthread_create(&threads[count++], NULL,mult, (void*)(data));
		}
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("%ds %dms\n",seconds, micros);
	for (int i = 0; i < M*P; i++){
	    void *k;
	    pthread_join(threads[i], &k);
		(*(*(MatC+i/M)+i%P)) = *(int *)k;
    }
    // for (int i = 0; i < M*P; i++)
	// {
	// void *k;
	
	// //Joining all threads and collecting return value
	// pthread_join(threads[i], &k);
			
		
	// 	int *p = (int *)k;
	// printf("%d ",*p);
	// if ((i + 1) % P == 0)
	// 	printf("\n");
	// }
    // for (int i = 0; i < M; i++){
	// 	for(int j = 0; j < P; j++)
	// 		printf("%d ",MatC[i][j]);
	// 	printf("\n");
	// }
    
}

int main(){
    int MatA[M][N];
    int MatB[N][P];
    int MatC[M][P];

    memset(MatC,0,M*P*sizeof(int));

    for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			MatA[i][j] = rand() % 10;
		
		// Generating random values in matB
	for (int i = 0; i < N; i++)
		for (int j = 0; j < P; j++)
			MatB[i][j] = rand() % 10;

    // int max = M*P;
    // pthread_t *threads;
	// threads = (pthread_t*)malloc(max*sizeof(pthread_t));
    multiplyMatrices(MatA,MatB,MatC);
    multiplyMatricesMultiThreads(MatA,MatB,MatC);
    return 0;
}