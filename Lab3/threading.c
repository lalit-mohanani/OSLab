#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void* myturn(void *arg){
    while(1){
        printf("my turn\n");
        sleep(1);
    }
    return NULL;
}

void urturn(){
    while (1){   
        printf("ur turn\n");
        sleep(2);
    }
    
}

int main(){
    pthread_t th;
    // myturn();
    pthread_create(&th,NULL,myturn,NULL);
    urturn();
    pthread_join(th,NULL);
    return 0;
}