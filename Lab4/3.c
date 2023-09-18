
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>

sem_t semEmpty;
sem_t semFull;
sem_t semMutex;
pthread_mutex_t ch1;
int c = 1;

int buffer[10];
int count =0;

void *producer(void *args){
    while (1)
    {
        // produce
        int x = rand()%100;
        
        sem_wait(&semEmpty);
        sem_wait(&semMutex);
        // Add to the buffer
        if(count<10){
            buffer[count]=x;
            count++;    
            printf("Produced %d\n",x);
        }

        sem_post(&semMutex);
        sem_post(&semFull);
        // sleep(1);
        // sleep(rand()%2);
    }    
}

void *consumer(void *thread_id){
    long tid = (long)thread_id;
    while(1){
        int y =-1;
        sem_wait(&semFull);
        sem_wait(&semMutex);
        // remove from the buffer
        if(count>0){
            y = buffer[count-1];
            count--;
            
        }
        printf("Consumed by %d value %d\n",tid,y);
        // sleep(1);
        sem_post(&semMutex);
        sem_post(&semEmpty);
        // sleep(rand()%5);
    }
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    int n=3; //16 people want to play
    pthread_t th[n];

    sem_init(&semEmpty,0,10); // 10 threads can pass
    sem_init(&semFull,0,0); // 0 threads can pass
    sem_init(&semMutex,0,1);
    pthread_mutex_init(&ch1, NULL) ;
    // pthread_mutex_init(&ch2, NULL) ;
    for(int i=0;i<n;i++){ 
        if(!i){
            if(pthread_create(th+i,NULL,&producer,NULL)){
                perror("Failed to create thread");
            }   
        }
        else{
            if(pthread_create(th+i,NULL,&consumer,(void *)i)){
                perror("Failed to create thread");
            }   
        }
    }
 
    for(int i=0;i<n;i++){
        if(pthread_join(th[i],NULL)){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&semMutex);
    
    return 0;
 
}