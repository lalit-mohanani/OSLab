#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <unistd.h>


#define NUM_RESOURCES 3
#define NUM_PROCESSES 3

int available[NUM_RESOURCES];
int maximum[NUM_PROCESSES][NUM_RESOURCES];
int allocation[NUM_PROCESSES][NUM_RESOURCES];
// sem_t s;
// 
// void sem_increase(sem_t *__sem,int val){
//     int i;
//     for(i=0;i<val;i++){
//         sem_post(__sem);
//     }
//     // sem_getvalue(__sem, &i);
//     // printf("sem value: %d\n", i);

// }
// void sem_decrease(sem_t *__sem,int val){
//     int i;
//     for(i=0;i<val;i++){
//         sem_wait(__sem);
//     }
//     // sem_getvalue(__sem, &i);
//     // printf("sem value: %d\n", i);
// }

int main(){
    // sem_init(&s,0,10);
    // sem_decrease(&s, 5);
    // sem_increase(&s, 100);
    // sem_destroy(&s);
    memset(allocation,0,sizeof(int)*NUM_RESOURCES*NUM_PROCESSES);
    for(int i=0;i<NUM_PROCESSES;i++){
        for(int j=0;j<NUM_RESOURCES;j++){
            printf("%d ",allocation[i][j]);
        }
        printf("\n");
    }
}