#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <semaphore.h>

sem_t a;
sem_t b;
sem_t c;

void *p1(void *args){

    sleep(1);
    printf("P1 done\n");

    sem_post(&a);
    pthread_exit(NULL);
}

void *p2(void *args){
    sem_wait(&a); 

    sleep(1);
    printf("P2 done\n");

    sem_post(&b);
    pthread_exit(NULL);
}

void *p3(void *args){
    sem_wait(&b);

    sleep(1);
    printf("P3 done\n");
    
    sem_post(&c);
    pthread_exit(NULL);
}

void *p4(void *args){
    sem_wait(&c);

    sleep(1);
    printf("P4 done\n");
    
    pthread_exit(NULL);
}

int main(int argc,char *argv[]){
    pthread_t th[4];
    sem_init(&a,1,0);
    sem_init(&b,1,0); 
    sem_init(&c,1,0);

    if(pthread_create(th,NULL,&p1,NULL)){
        perror("Failed to create thread");
    }
    if(pthread_create(th+1,NULL,&p2,NULL)){
        perror("Failed to create thread");
    }
    if(pthread_create(th+2,NULL,&p3,NULL)){
        perror("Failed to create thread");
    }
    if(pthread_create(th+3,NULL,&p4,NULL)){
        perror("Failed to create thread");
    }
 
    for(int i=0;i<4;i++){
        if(pthread_join(th[i],NULL)){
            perror("Failed to join thread");
        }
    }

    sem_destroy(&a);
    sem_destroy(&b);
    sem_destroy(&c);
    
    return 0;
 
}