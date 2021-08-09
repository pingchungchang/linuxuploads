#include <fcntl.h> // include open() function
#include <sys/types.h> // include mode_t class&wait&fifo
#include <sys/stat.h> // flag of open function&fifo
#include <unistd.h> // closing fd&read&write&fork&execl&pipe
#include <stdlib.h> // stdio
#include <sys/wait.h> // wait

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdio>
#include <stdio.h>
#include <math.h>
using namespace std;

int main(){
    int mainfifo = open("fifop",O_RDONLY);
    char* ids = new char(1);
    printf("whatever");
    while(read(mainfifo,ids,sizeof(char))<=0);
    int wins = 0;
    printf("%s",ids);
    int id = ids[0] - '0';
    char* fifoname = new char(5);
    char* fifoname2 = new char(6);
    fifoname2[5] = '1';
    sprintf(fifoname,"fifo%d",id);
    for(int i =0;i<5;i++){
        fifoname2[i] = fifoname[i];
    }
    int wr = open(fifoname,O_WRONLY);
    int re = open(fifoname2,O_RDONLY);
    for(int i =0;i<10;i++){
        srand(time(NULL)^(getpid()<<19));
        int number = rand()%10;
        char* num = new char(1);
        num[0] = '0'+number;
        if(write(wr,num,sizeof(char)) <=0){
            cout<<"write"<<i<<"error";
            return 0;
        }
        while(read(re,fifoname,5*sizeof(char)) <=0);//did not work
        printf("%s\n",fifoname);
        if(fifoname[id] == '1'){
            wins++;
        }
        printf("Player%d wins %d times\n",id,wins);
    }
    char* wintimes = new char(1);
    wintimes[0] ='0'+ wins;
    if(write(wr,&wintimes,sizeof(char)) <=0){
        cout<<"write result error";
        return 0;
    }
    close(wr);
    close(re);
    return 0;
}
