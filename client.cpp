//client.cpp

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) printf("Socket Create Error\n");
    struct sockaddr_in info;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1"); // Local IP Address
    info.sin_port = htons(8080); // Port 8080
    int connect_result = connect(sockfd, (struct sockaddr*)&info, sizeof(info));
    if(connect_result) printf("Connect Error\n");
    char*c = new char(100);
    sprintf(c,"whatever\n");
    send(sockfd,c,sizeof(c),0);
    recv(sockfd,c,sizeof(c),0);
    printf("%s",c);
}
