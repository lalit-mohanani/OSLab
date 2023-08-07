#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    // int y = fork();
    // int x = fork();
    // // if(x==0){
    // //     printf("child");
    // //     printf("%d",x);
    // //     exit(0);
    // // }
    // // else{
    // //     printf("parent");
    // //     printf("%d",x);
    // //     exit(0);
    // // }
    // printf("hii %d %d\n",x,y);
    // exit(0);
    // return 0;
    // if(fork()>0){
    //     printf("parent");
    //     exit(0);
    // }
    // else if(fork()==0){
    //     printf("child");
    //     exit(0);
    // }
    // else{
    //     printf("err");
    //     exit(0);
    // }
    // for(int i=0;i<2;i++){
    //     fork();
    //     printf("Hii");
    // }
    printf("HII\n");
    fork();
    return 0;
}
