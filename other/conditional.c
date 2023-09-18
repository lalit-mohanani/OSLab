#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

int fuel = 0;
pthread_mutex_t mut;
pthread_cond_t condfuel;

void *fuel_filling(void *arg)
{
    for (int i = 0; i < 5; ++i)
    {
        pthread_mutex_lock(&mut);
        fuel += 15;
        printf("filling fuel... %d \n",fuel);
        pthread_mutex_unlock(&mut);
        pthread_cond_signal(&condfuel);//signals all the threads that are waiting for the below condition
        sleep(1);
    }
}

void *car(void *arg)
{
    pthread_mutex_lock(&mut);
    while(fuel < 40)
    {
        printf("no fuel left. waiting......\n");
        pthread_cond_wait(&condfuel,&mut);//waits for a signal from another thread that may or maynot change the condition
        //equiavalent to :
        //pthread_mutex_unlock(&mut)
        //wait for signal on condfuel
        //pthread_mutex_lock(&mut)
    }
    fuel -= 40;
    printf("got fuel left with... %d \n",fuel);
    pthread_mutex_unlock(&mut);
}

int main()
{
    pthread_t th[2];
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&condfuel,NULL);

    for (int i = 0; i < 2; ++i)
    {
        if (i == 1)
        {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0)
                perror("failed");
        }

        if (i == 0)
        {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0)
                perror("failed");
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        if (pthread_join(th[i], NULL) != 0)
            perror("failed to join");
    }

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&condfuel);
    return 0;
}