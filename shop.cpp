#include <fcntl.h> // include open() function
#include <sys/stat.h> // flag of open function&fifo
#include <sys/types.h> // include mode_t class&wait&fifo
#include <unistd.h> // closing fd&read&write&fork&execl&pipe
#include <stdlib.h> // stdio
#include <sys/wait.h> // wait
#include <sys/select.h>//select
#include <iostream>
#include <stdio.h>
#include <memory.h>//memcpy
#include <string.h>
using namespace std;

int main(){
    //int op = open("shopmainfifo",O_WRONLY);
    //int w = write(op,"012",3*sizeof(char));
    //if(w<=0) return 999;
    fd_set readset,setcopy;
    FD_ZERO(&readset);
    timeval timeout;
    int goods[3] = {5,5,5};
    int readfifos[3];
    int writefifos[3];
    for(int i = 0;i<3;i++){
        char* reads = new char(100);
        sprintf(reads,"shopfifo%d",i);
        readfifos[i] = open(reads,O_RDONLY);
        if(readfifos[i] < 0){
            printf("open read %d error",i);
            return 0;
        }
        FD_SET(readfifos[i],&readset);
        sprintf(reads,"shopfifo0%d",i);
        writefifos[i] = open(reads,O_WRONLY);
        if(writefifos[i] < 0){
            printf("open write%d error",i);
            return 0;
        }
    }

    while(goods[0]+goods[1]+goods[2]>0){
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        memcpy(&setcopy,&readset,sizeof(readset));
        select(10,&setcopy,NULL,NULL,&timeout);
        for(int i = 0;i<3;i++){
            if(FD_ISSET(readfifos[i],&setcopy)){
                char* c = new char(1);
                int re = read(readfifos[i],c,sizeof(c));
                if(re <=0){
                    printf("read error");
                    return 0;
                }
                int what = c[0]-'0';
                if(goods[what] <=0){
                    int wr = write(writefifos[i],"0",sizeof(char));
                    if(wr<=0){
                        printf("write error");
                        return 0;
                    }
                }
                else{
                    goods[what] --;
                    if(what == 0){
                        printf("sell Apple to Costomer %d\n",i);
                    }
                    else if(what == 1){
                        printf("sell Banana to Costomer %d\n",i);
                    }
                    else{
                        printf("sell Orange to Costomer %d\n",i);
                    }
                    int wr = write(writefifos[i],"1",sizeof(char));
                }
            }
        }
    }
}
