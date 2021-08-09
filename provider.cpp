#include <iostream>
#include <string.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

int main(){
    char* s = new char(100);
    char* a = new char(100);
    char* b = new char(100);
    int fd = open("/home/chang/000.cpp/tmp.txt",O_CREAT|O_RDWR,0777);
    scanf("%s",s);
    if(fd == -1){
        cout<<"fd fail";
        return 0;
    }
    if(strcmp(s,"writeNum") == 0){
        scanf("%s %s",a,b);
        int wr = write(fd,a,sizeof(a));
        if(wr==-1){
            cout<<"write a fail";
            return 0;
        }
        wr = write(fd," ",sizeof(char));
        if(wr == -1){
            cout<<"write space fail";
            return 0;
        }
        wr = write(fd,b,sizeof(b));
        cout<<wr;
        if(wr == -1){
            cout<<"b fail";
            return 0;
        }
        char* space = " ";
        wr = write(fd,space,sizeof(char));
        if(wr == -1){
            cout<<"write second space fail";
            return 0;
        }
        cout<<"input";
        int ls = lseek(fd,0,SEEK_SET);
        if(ls == -1){
            cout<<"ls fail";
            return 0;
        }
        close(fd);
    }
    else if(strcmp(s,"readAns") == 0){
        char* c = new char(100);
        int r = read(fd,c,100*sizeof(char));
        if(r == -1){
            cout<<"readfail";
            return 0;
        }
        //cout<<r;
        int cnt = 0;
        int position = 0;
        while(cnt<=1){
            position ++;
            if(c[position] == ' ') cnt++;
        }
        position ++;
        while(position <r){
            printf("%c",c[position]);
            position++;
        }
    }
    close(fd);
}
