#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>


int main(){
    struct timeval start, end;
    int x,y;
    scanf("%d",&x);
    scanf("%d",&y);
    gettimeofday(&start, NULL);
    printf("operation     pid   value time\n");
    for(int i=0;i<4;i++){
        if(fork()==0){
            if(i==0){
                printf("addition      %d  %d ",getpid(),x+y);
            }
            else if(i==1){
                printf("subtraction   %d  %d ",getpid(),x-y);
            }
            else if(i==2){
                printf("multipication %d  %d ",getpid(),x*y);
            }
            else{
                printf("division      %d  %d ",getpid(),x/y);
            }
            gettimeofday(&end, NULL);
            long seconds = (end.tv_sec - start.tv_sec);
            long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("%ds %dms\n",seconds, micros);
            break;
        }
    }
    return 0;
}
