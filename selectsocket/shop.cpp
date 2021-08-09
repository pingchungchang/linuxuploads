#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
using namespace std;


int main(){
    int whatever = 3;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) printf("Socket Create Error\n");
    struct sockaddr_in info;
    struct sockaddr_in client_info;
    socklen_t client_addr_len;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY; // Local IP Address
    info.sin_port = htons(8080); // Port 8080
    int bind_result = bind(sockfd, (struct sockaddr*)&info, sizeof(info));
    if(bind_result < 0) printf("Bind Error\n");
    int client_fd[3] = {0};
    for(int i = 0;i<3;i++){
        whatever++;
       // printf("%d",i);
        listen(sockfd, 3); // block until listen a client
        client_fd[i] = accept(sockfd, (struct sockaddr*)&client_info, &client_addr_len);
        char* id = new char(10);
        sprintf(id,"%d",i+1);
        int j = send(client_fd[i],id,10*sizeof(char),0);
        printf("%d",j);
        printf("Accept client, client port: %d\n", client_info.sin_port);
    }
    //printf("started");
    fd_set initial;
    FD_ZERO(&initial);
    for(int i = 0;i<=3;i++){
        FD_SET(client_fd[i],&initial);
    }
    char names[3][10] = {"oranges","bananas","apples"};
    int goods[3] = {5,5,5};
    while(goods[0]+goods[1]+goods[2] > 0){
        fd_set readset;
        memcpy(&readset,&initial,sizeof(initial));
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 2;
    //    for(int i = 0;i<3;i++)printf("%d",client_fd[i]);
        select(whatever+1,&readset,NULL,NULL,&timeout);
        for(int i = 0;i<3;i++){
            if(FD_ISSET(client_fd[i],&readset)){
                char* buying = new char(10);
                if(recv(client_fd[i],buying,10*sizeof(char),0)<=0)return i+10;
            //    printf("buying = %s",buying);
                int stuff = buying[0]-'0';
       //         printf("stuff is %d\n",stuff);
                if(goods[stuff]>0){
                    goods[stuff]--;
                    printf("Sell %s to Costumer %d\n",names[stuff],i+1);
                    if(send(client_fd[i],buying,10*sizeof(char),0)<=0)return i + 5;
                }
                else{
                    if(send(client_fd[i],"9",10*sizeof(char),0)<=0)return i +1;
                }
            }
        }
    }
    for(int i = 0;i<3;i++)close(client_fd[i]);
    close(sockfd);
    return 0;
}
