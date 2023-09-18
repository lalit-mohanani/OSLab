#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int arr[5] = {1,2,3,4,5};

void* routine(void *arg)
{
    int index = *(int*)arg;
    printf("%d ",arr[index]);
    free(arg);
}

int main(int argc,char* argv[])
{
   /* pthread_t th[5];

    for(int i = 0 ; i < 5; ++i)
    {
        if(pthread_create(&th[i],NULL,&routine,&i)!=0)
        return 1;
    }

    for(int i = 0 ; i < 5; ++i)
    {
        if(pthread_join(th[i],NULL)!=0)
        return 2;
    }
    printf("\n");*/

    //from the above code we arent able to print unique values
    //assume the first thread is  created but hasnt started.
    //so now i value changes and the second thread gets i = 1
    //now i=1 is for thread 1 and thread 2 because we are passing
    //by refernce so value change for thread 2 effects thread 1


    //solution dynamically allocate the passing value for each thread

    pthread_t th[5];

    for(int i = 0 ; i < 5; ++i)
    {   int *a = malloc(sizeof(int));
        *a=i;
        if(pthread_create(&th[i],NULL,&routine,a)!=0)
        return 1;
    }

    for(int i = 0 ; i < 5; ++i)
    {
        if(pthread_join(th[i],NULL)!=0)
        return 2;
    }
    printf("\n");
}