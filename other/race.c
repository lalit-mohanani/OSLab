#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
int lock = 0;
void *routine()
{   for(int i = 0 ; i < 100000; ++i)
    {   lock = 1;
        mails++;
        lock = 0;
        //read mails
        //increment
        //write mails 
    }
}

int main(int argc, char* argv[])
{
     pthread_t t1,t2;
    if(pthread_create(&t1,NULL,&routine,NULL)!=0)
    return 1;;// THE LAST PARAMETER IS THE ARGUMENT TO ROUTINE
    
    if(pthread_create(&t2,NULL,&routine,NULL)!=0)
    return 2;;

    if(pthread_join(t1,NULL)!=0)
    return 3;//JUST LIKE WAIT BUT FOR THREADS 
    
    if(pthread_join(t2,NULL)!=0)
    return 4;

    printf("number of mails: %d\n",mails);
    return 0;
}