#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>

pthread_t th[4];

void *p1(void *args){

    sleep(1);
    printf("P1 done\n");

    pthread_exit(NULL);
}

void *p2(void *args){

    if(pthread_join(th[0],NULL)){
        perror("Failed to join thread");
    }
    
    sleep(1);
    printf("P2 done\n");

    pthread_exit(NULL);
    
}

void *p3(void *args){
    if(pthread_join(th[1],NULL)){
        perror("Failed to join thread");
    }

    sleep(1);
    printf("P3 done\n");
    
    pthread_exit(NULL);
}

void *p4(void *args){
    if(pthread_join(th[2],NULL)){
        perror("Failed to join thread");
    }

    sleep(1);
    printf("P4 done\n");
    
    pthread_exit(NULL);
}

int main(int argc,char *argv[]){

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

    if(pthread_join(th[3],NULL)){
        perror("Failed to join thread");
    }
    
    return 0;
 
}