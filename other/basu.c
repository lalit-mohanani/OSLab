//Solve the bounded buffer problem using semaphores

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 15

int buffer[BUFFER_SIZE];
int in=0;
int out=0;
int prodno=0;
int consno=0;

sem_t mutex1;
sem_t mutex2;
int turn=-1;
int flag1=0;
int flag2=0;

void* prodfunc(void* arg){
    int item=1;
    while(prodno<MAX_ITEMS){
        sem_wait(&mutex1);
        buffer[in]=item;
        printf("Produced:%d\n",item);
        item++;
        in=(in+1)%BUFFER_SIZE;
        prodno++;
        sem_post(&mutex2);
    }
    pthread_exit(NULL);
}

void* consfunc1(void* arg){
    while(consno<MAX_ITEMS){
        sem_wait(&mutex2);
        int item=buffer[out];
        if(turn==1 && flag1==0){
            flag1++;
        }
        else if(turn==1 && flag1==1){
            flag1--;
            turn=(turn+1)%3;
        }
        else if(turn==2 && flag2<2){
            flag2++;
        }
        else if(turn==2 && flag2==2){
            flag2=0;
            turn=(turn+1)%3;
        }
        else{
            turn=(turn+1)%3;
        }
        
        printf("Consumed by cons %d:%d\n",turn+1,item);
        out=(out+1)%BUFFER_SIZE;
        consno++;
        sem_post(&mutex1);
       // sleep(1+rand()%3);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t producer,consumer;
    if(sem_init(&mutex1,1,1)!=0) printf("Mutex1 semaphore cant be initialised\n");
    if(sem_init(&mutex2,1,0)!=0) printf("Mutex2 semaphore cant be initialised\n");

    if(pthread_create(&producer,NULL,prodfunc,NULL)!=0) printf("Producer thread cant be created\n");
    if(pthread_create(&consumer,NULL,consfunc1,NULL)!=0) printf("Consumer thread cant be created\n");
    

    if(pthread_join(producer,NULL)!=0) printf("Producer thread cant be joined\n");
    if(pthread_join(consumer,NULL)!=0) printf("Consumer thread cant be joined\n");
    
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);
}

