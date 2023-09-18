#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
int lock = 0;
pthread_mutex_t mutex;

void *routine()
{
    for (int i = 0; i < 10000000; ++i)
    {
        pthread_mutex_lock(&mutex);//lock = 1
       /* if(lock == 1)
        {
            //wait until lock is 0
        }*/
        mails++;  //only one thread executes inside this mutex enclosement
        //lock = 0;
        pthread_mutex_unlock(&mutex);//lock = 0
        // read mails
        // increment
        // write mails
    }
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex,NULL);
    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
        return 1;
    ; // THE LAST PARAMETER IS THE ARGUMENT TO ROUTINE

    if (pthread_create(&t2, NULL, &routine, NULL) != 0)
        return 2;
    ;

    if (pthread_join(t1, NULL) != 0)
        return 3; // JUST LIKE WAIT BUT FOR THREADS

    if (pthread_join(t2, NULL) != 0)
        return 4;
    
    pthread_mutex_destroy(&mutex);

    printf("number of mails: %d\n", mails);
    return 0;
}