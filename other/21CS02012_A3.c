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
sem_t ch;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count =0;
int turn  = 0;
int c11=1,c1 = 1;//consumer 0 will run 1 time
int c21 = 2,c2 = 2;//consumer2 will 2 times 
int c31=1,c3 = 1;//consumer3 will run 3 times

void *producer(void *args){
    while (1)
    {
        // produce
        sem_wait(&ch);

        int x = rand()%100;

        sem_wait(&semEmpty);

        pthread_mutex_lock(&mutexBuffer);
        // Add to the buffer
        if(count<10){
            buffer[count]=x;
            printf("produced %d \n",x);
            count++;    
        } else{
            printf("Skipped %d\n",x);
        }
        pthread_mutex_unlock(&mutexBuffer);
        
        sem_post(&semFull);
       
       
    }    
}

void *consumer(void *args){
    int index = (int) args;
    while(1){
       
        while(index!=turn+1){;}

        int y =-1;

        sem_wait(&semFull);

        pthread_mutex_lock(&mutexBuffer);
        
        // remove from the buffer
        if(count>0){
            y = buffer[count-1];
            count--;
            
        }
        printf("got element %d from %d\n",y,turn);
        
        
        if(index == 1)
        {
            c1--;
            if(c1 == 0)
            {
                c1=c11;
                turn = (turn+1)%3;
            }
        }

        if(index == 3)
        {
            c3--;
            if(c3 == 0)
            {
                c3=c31;
                turn = (turn+1)%3;
            }
        }
        
        else if(index == 2)
        {
            c2--;
            if(c2 == 0)
            {
                c2=c21;
                turn = (turn+1)%3;
            }
        }
        
        pthread_mutex_unlock(&mutexBuffer);
        
        sem_post(&semEmpty);
        sem_post(&ch);
       
    }
}

int main(int argc,char *argv[]){
    srand(time(NULL));
    int n=4; 
    pthread_t th[n];
    pthread_mutex_init(&mutexBuffer,NULL);
    sem_init(&semEmpty,0,10); // 10 threads can pass
    sem_init(&semFull,0,0); // 0 threads can pass
    sem_init(&ch,0,1);
    
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

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);

    pthread_mutex_destroy(&mutexBuffer);
    
    return 0;
 
}