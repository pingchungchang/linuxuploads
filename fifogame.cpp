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
#include <stdio.h>
using namespace std;

int main(){
    printf("readthis");
    char* c = new char(5);
    for(int i =0;i<5;i++){
        c[i] = '0'+i;
    }
    printf("whatever");
    int mainfifo = open("fifop",O_WRONLY);
    printf("didyoureadthis");
    if(mainfifo == -1) return 1;
    int wr = write(mainfifo,c,5*sizeof(char));
    vector<int> reads;
    reads.push_back(open("fifo0",O_RDONLY));
    reads.push_back(open("fifo1",O_RDONLY));
    reads.push_back(open("fifo2",O_RDONLY));
    reads.push_back(open("fifo3",O_RDONLY));
    reads.push_back(open("fifo4",O_RDONLY));
    vector<int> writes;
    printf("stuck?");
    writes.push_back(open("fifo01",O_WRONLY));
    writes.push_back(open("fifo11",O_WRONLY));
    writes.push_back(open("fifo21",O_WRONLY));
    writes.push_back(open("fifo31",O_WRONLY));
    writes.push_back(open("fifo41",O_WRONLY));
    for(int i =0;i<5;i++){
        if(reads[i] == -1||writes[i] == -1){
            cout<<"open"<<i<<"error";
            return 0;
        }
    }
    for(int i =0;i<10;i++){
        unordered_map<int,vector<int>> winners;
        int win = -1;
        int abc = i+1;
        printf("Round%d :\n",abc);
        for(int j =0;j<5;j++){
            char * numbers = new char(100);
            while(read(reads[j],numbers,sizeof(char)) <=0);
            int k = numbers[0]-'0';
            printf("Player%d : %d\n",j,k);
            winners[k].push_back(j);
            win = max(win,k);
        }
        char* winsend = new char(5);
        for(int i =0;i<5;i++){
            winsend[i] = '0';
        }
        for(int j = 0;j<winners[win].size();j++){
            winsend[winners[win][j]] = '1';
            printf("Player%d wins round%d\n",winners[win][j],i+1);
        }
        for(int j = 0;j<5;j++){
            int wr = write(writes[j],winsend,5*sizeof(char));
        }
    }
    char* fin = new char(100);
    unordered_map<int,vector<int>> wintimes;
    priority_queue<int,vector<int>,less<int>> pq;
    for(int i = 0;i<5;i++){
        while(read(reads[i],fin,sizeof(char))<=0);
        if(wintimes.count(fin[0] - '0') ==0){
            pq.push(fin[0]-'0');
        }
        wintimes[fin[0]-'0'].push_back(i);
    }
    printf("result\n");
    while(!pq.empty()){
        for(int i =0;i<wintimes[pq.top()].size();i++){
            printf("Player%d wins %d times\n",wintimes[pq.top()][i],pq.top());
        }
        pq.pop();
    }
    for(int i =0;i<5;i++){
        close(reads[i]);
        close(writes[i]);
    }
    return 0;
}
