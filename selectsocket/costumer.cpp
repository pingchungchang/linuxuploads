# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
#include <algorithm>
#include <ctime>
#include <math.h>
#include <string.h>
using namespace std;
int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) printf("Socket Create Error\n");
    struct sockaddr_in info;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1"); // Local IP Address
    info.sin_port = htons(8080); // Port 8080
    int connect_result = connect(sockfd, (struct sockaddr*)&info, sizeof(info));
    if(connect_result) printf("Connect Error\n");
    char* idchar = new char(10);
    if(recv(sockfd,idchar,10*sizeof(char),0) <=0)return 5;
    int selfid = idchar[0]-'0';
    char things[3][10] = {"orange","banana","apple"};
    printf("started");
    while(true){
        srand(time(NULL)^(getpid()<<19));
        int number = rand()%10+1;
        usleep(100000*number);
        int k = rand()%3;
        char* buying = new char(10);
        sprintf(buying,"%d",k);
        //printf("wadfas%s",buying);
        if(send(sockfd,buying,10*sizeof(char),0)<=0)return 4;
        char* result = new char(10);
        if(recv(sockfd,result,10*sizeof(char),0)<=0)return 3;
        if(result[0] == '9'){
            printf("Costumer %d buys 0\n",selfid);
        }
        else{
            printf("Costumer %d buys 1 %s\n",selfid,things[result[0]-'0']);
        }
    }
}
