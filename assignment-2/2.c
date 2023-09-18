#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>

sem_t empty;
sem_t full;
sem_t chance;

pthread_mutex_t mutex;

int buffer[10];
int count =0;
int turn  = 0;
int c11=1,c1 = 2;
int c21=1,c2 = 1;
int c31=1,c3 = 1;

void *producer(void *args){
    while (1)
    {
        // produce
        sem_wait(&chance);

        int x = rand()%100;

        sem_wait(&empty);

        pthread_mutex_lock(&mutex);
        // Add to the buffer
        if(count<10){
            buffer[count]=x;
            printf("produced %d \n",x);
            count++;    
        } else{
            printf("Skipped %d\n",x);
        }
        sleep(1);
        pthread_mutex_unlock(&mutex);
        
        sem_post(&full);
       
       
    }    
}

void *consumer(void *args){
    int id = (int) args;
    while(1){

        while(id!=turn+1);
        int y =-1;
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        //for consumer 0
        if(id==1){
            c1--;
            if(c1 == 0){
                c1=c11;
                turn = (turn+1)%3;
            }
        } // for consumer 1
        else if (id == 2){
            c2--;
            if(c2 == 0){
                c2=c21;
                turn = (turn+1)%3;
            }
        } //for consumer 2
        else{
            c3--;
            if(c3 == 0){
                c3=c31;
                turn = (turn+1)%3;
            }
        }
        if(count>0){
            y = buffer[count-1];
            count--;
        }
        printf("C%d consumed value %d\n",turn,y);
        pthread_mutex_unlock(&mutex);
        
        sem_post(&empty);
        sem_post(&chance);
       
    }
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    printf("Enter no of times each consumer consume in one cycle [0-10]\n");
    printf("For C0\n");
    scanf("%d", &c1);
    printf("For C1\n");
    scanf("%d", &c2);
    printf("For C2\n");
    scanf("%d", &c3);
    c11 = c1;
    c21 = c2;
    c31 = c3;
    int n=4; 
    pthread_t th[n];
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,10); 
    sem_init(&full,0,0); 
    sem_init(&chance,0,1);
    
    for(int i=0;i<n;i++){ 
        if(!i){
            if(pthread_create(th+i,NULL,&producer,NULL)){
                perror("Failed to create thread");
            }   
        }
        else{
            if(pthread_create(th+i,NULL,&consumer,i)){
                perror("Failed to create thread");
            }   
        }
    }
 
    for(int i=0;i<n;i++){
        if(pthread_join(th[i],NULL)){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&chance);
    pthread_mutex_destroy(&mutex);
    
    return 0;
 
}