// server.cpp
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
    struct sockaddr_in client_info;
    socklen_t client_addr_len;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY; // Local IP Address
    info.sin_port = htons(8080); // Port 8080
    int bind_result = bind(sockfd, (struct sockaddr*)&info, sizeof(info));
    if(bind_result < 0) printf("Bind Error\n");
    listen(sockfd, 1); // block until listen a client
    int client_fd;
    while(1){
        client_fd = accept(sockfd, (struct sockaddr*)&client_info, &client_addr_len);
        printf("Accept client, client port: %d\n", client_info.sin_port);
        char* s = new char(100);
        recv(client_fd,s,sizeof(s),0);
        printf("%s\n",s);
        char respond[100] = "recieved";
        send(client_fd,&respond,sizeof(respond),0);
    }
    return 0;
}
