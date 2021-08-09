#include <fcntl.h> // include open() function
#include <sys/types.h> // include mode_t class&wait&fifo
#include <sys/stat.h> // flag of open function&fifo
#include <unistd.h> // closing fd&read&write&fork&execl&pipe
#include <stdlib.h> // stdio
#include <sys/wait.h> // wait
#include <sys/select.h>//select
#include <time.h>
#include <stdio.h>
#include <string.h>
using namespace std;

int main(){

    //int mainfifo = open("shopmainfifo",O_RDONLY);
    //if(mainfifo<=0) return 876;
    int id;
    //while(int whatever = read(mainfifo,id,sizeof(id)) <= 0){
      //  printf("%d\n",whatever);
    //}
    scanf("%d",&id);
    printf("%d",id);
    char* readname = new char(100);
    sprintf(readname,"shopfifo0%d",id);
    char* writename = new char(90);
    sprintf(writename,"shopfifo%d",id);
    int wr = open(writename,O_WRONLY);
    if(wr <=0) return 7;
    int re = open(readname,O_RDONLY);
    if(re <= 0) return 8;
    srand(time(NULL)^(getpid()<<19));
    while(1){
        int sleeptime = rand()%11;
        while(sleeptime == 0){
            sleeptime = rand()%11;
        }
        usleep(sleeptime*100000);
        int buy = rand()%3;
        char* item = new char(100);
        sprintf(item,"%d",buy);
        int writefifo = write(wr,item,strlen(item));
        if(writefifo<=0)return 99;
        char* suc = new char(100);
        while(read(re,suc,sizeof(char)) == 0);
        int number = 0;
        if(suc[0] == '1') number = 1;
        if(item[0] == '0'){
            printf("Customer %d buys %d Apples\n",id,number);
        }
        else if(item[0] == '1'){
            printf("Customer %d buys %d Bananas\n",id,number);
        }
        else if(item[0] == '2'){
            printf("Customer %d buys %d Oranges\n",id,number);
        }
    }
}
