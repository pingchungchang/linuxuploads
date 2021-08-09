#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <ctime>
using namespace std;
int host(vector<int> reads,vector<int> writes,vector<int> ids){
    vector<int> winningtimes(5,0);
    for(int l =0;l<10;l++){
        vector<int> nums;
        for(int i =0;i<5;i++){
            char* c = new char(1);
            read(reads[i],c,sizeof(char));
            nums.push_back(c[0]-'0');
        }
        queue<int> winners;
        int biggest;
        biggest = nums[0];
        for(int i  =0;i<5;i++){
            //cout<<nums[i]<<' ';
            //cout<<biggest<<" "<<i;
            if(biggest==nums[i]){
                winners.push(i);
            }
            else if(biggest<nums[i]){
                while(!winners.empty()){
                    winners.pop();
                }
                biggest = nums[i];
                winners.push(i);
            }
        }
        queue<int> winners2 = winners;
        for(int i =0;i<5;i++){
            if(!winners.empty()&&winners.front() == i){
                winningtimes[winners.front()] ++;
                write(writes[i],"1",sizeof(char));
                winners.pop();
            }
            else{
                write(writes[i],"0",sizeof(char));
            }
        }
        for(int i =0;i<5;i++){
            printf("Player%d: %d\n",i,nums[i]);
        }
        while(!winners2.empty()){
            printf("Player%d wins round%d\n",winners2.front(),l);
            winners2.pop();
        }
    }
    unordered_map<int,vector<int>> wintoid;
    for(int i =0;i<winningtimes.size();i++){
        wintoid[winningtimes[i]].push_back(i);
    }
    sort(winningtimes.begin(),winningtimes.end());
    for(int i =4;i>=0;i--){
        if(wintoid[winningtimes[i]][0] !=-1){
            for(int f = 0;f<wintoid[winningtimes[i]].size();f++){
                printf("Player%d, win %d times\n",wintoid[winningtimes[i]][f],winningtimes[i]);
            }
            wintoid[winningtimes[i]] = {-1,-1,-1};
        }
    }
    for(int i = 0;i<5;i++){
        close(reads[i]);
        close(writes[i]);
    }
    return 0;
}
int player(int readpipe,int writepipe,int id){
    int wins = 0;
    //cout<<getpid()<<' ';
    for(int i =0;i<10;i++){
        int w;
        srand(time(NULL)^(getpid()<<19));
        w = rand()%10;
        w = w%10;
        char num = w+'0';
        write(writepipe,&num,sizeof(char));
        char* r = new char(100);
        read(readpipe,r,sizeof(char));
        if(r[0] == '1') wins++;
        sleep(1);
    }
    close(readpipe);
    close(writepipe);
    return 0;
}
int main(){
    vector<pid_t> pids;
    vector<int> readpipes;
    vector<int> writepipes;
    for(int i =0;i<5;i++){
        int fd1[2];
        int fd2[2];
        int j = pipe(fd1);
        int k = pipe(fd2);
        if(j == -1||k == -1){
            printf("pipefail");
            return 0;
        }
        pid_t PID = fork();
        if(PID == 0){
            int play = player(fd1[0],fd2[1],i);
            return 0;
        }
        else{
            readpipes.push_back(fd2[0]);
            writepipes.push_back(fd1[1]);
            pids.push_back(PID);
        }
    }
    host(readpipes,writepipes,pids);
    for(int i =0;i<5;i++){
        wait(nullptr);
    }
    cout<<"Game Over";
    return 0;
}