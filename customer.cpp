#include <iostream>
#include <string.h>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
using namespace std;
int main(){
    char* input = new char(100);
    scanf("%s",input);
    if(strcmp(input,"calAns") == 0){
        char* s = new char(100);
        int fd = open("/home/chang/000.cpp/tmp.txt",O_CREAT|O_RDWR,0777);
        if(fd == -1){
        cout<<"fd fail";
        return 0;
        }
        int r = read(fd,s,100*sizeof(char));
        if(r == -1){
        cout<<"read fail";
        return 0;
        }
        int spaces = 0;
        string s1;
        string s2;
        int i =0;
        while(s[i] !=' '){
            s1 +=s[i];
            i++;
        }
        for(int j = i+1;i<r;i++){
            s2 += s[i];
        }
        int n1 = stoi(s1);
        int n2 = stoi(s2);
        int n3 = n1+n2;
        string sf = to_string(n3);
        char * c = new char(sf.size());
        for(int i = 0;i<sf.size();i++){
            c[i] = sf[i];
        }
        int ls = lseek(fd,0,SEEK_END);
        if(ls == -1){
            cout<<"ls fail";
            return 0;
        }
        int w = write(fd,c,sf.size()*sizeof(char));
        if(w == -1){
            cout<<"write fail";
            return 0;
        }
        cout<<"success";
        close(fd);
    }
}
