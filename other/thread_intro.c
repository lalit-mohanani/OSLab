#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x = 2;
void* routine()
{
   x++;
   sleep(2);
   printf("x is %d\n",x);
}

void* routine2()
{
   sleep(2);
   printf("x is %d\n",x);
}

//threads sharing memory demonstration

int main(int argc, char* argv[])
{
    pthread_t t1,t2;
    if(pthread_create(&t1,NULL,&routine,NULL)!=0)
    return 1;;// THE LAST PARAMETER IS THE ARGUMENT TO ROUTINE
    
    if(pthread_create(&t2,NULL,&routine2,NULL)!=0)
    return 2;;

    if(pthread_join(t1,NULL)!=0)
    return 3;//JUST LIKE WAIT BUT FOR THREADS 
    
    if(pthread_join(t2,NULL)!=0)
    return 4;

    printf("hello\n");
    return 0;
}