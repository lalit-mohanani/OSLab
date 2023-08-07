#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>


int main(){
    struct timeval start, end;
    int x,y;
    // scanf("%d",&x);
    // scanf("%d",&y);
    // gettimeofday(&start, NULL);
    // printf("%d\n",getpid());
    for(int i=0;i<2;i++){
        if(fork()==0){
            for(int j=0;j<2;j++){
                if(fork()==0){
                    printf("GC \n");
                    break;
                }
            }
            break;
        }
    }
    // if(getpid()>0){
    //     for(int i=0;i<2;i++){
    //         if(fork()==0){
    //             printf("GC\n");
    //             break;
    //         }
    //     }
    // }
    return 0;
}
