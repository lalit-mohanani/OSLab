#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *roll()
{
    int x = 3;
    int *result = malloc(sizeof(int));
    *result = x;
    return (void*)result;
}
int main()
{   int *res;
    pthread_t th;
    if(pthread_create(& th,NULL,&roll,NULL)!=0)
    return 1;

    if(pthread_join(th,(void**)&res)!=0)
    return 2;

    printf("%d\n",*res);
    free(res);

    return 0;
}