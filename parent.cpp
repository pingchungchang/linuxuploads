#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <string.h>
using namespace std;
int player(int fd, int id){
    char* s = new char(100);
    int re = read(fd,s,7*sizeof(char));
    for(int i = 0;i<re;i++){
        cout<<s[i]<<"\n";
    }
}
int parent(int fd){
    int wr = write(fd,"hihihi\n",7*sizeof(char));
}
int main(){
    int fd[2];
    int pipework = pipe(fd);
    if(pipework != 0){
        cout<<"pipe fail";
        return 0;
    }
    //fd[0] : write
    //fd[1] : read
    pid_t PID;
    for(int i =0;i<5;i++){
        PID = fork();
        if(PID == 0){
            close(fd[1]);
            player(fd[0],i);
            exit(0);
        }
    }
    if(PID != 0){
        close(fd[0]);
        parent(fd[1]);
    }


}
