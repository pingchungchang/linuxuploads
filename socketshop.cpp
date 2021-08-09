#include <fcntl.h> // include open() function
#include <sys/types.h> // include mode_t class&wait&fifo
#include <sys/stat.h> // flag of open function&fifo
#include <unistd.h> // closing fd&read&write&fork&execl&pipe
#include <stdlib.h> // stdio
#include <sys/wait.h> // wait
#include <sys/select.h>//select
#include <sys/socket.h>//socket&bind&&listen&accept&recv&connect&send
# include <netinet/in.h>//bind
# include <arpa/inet.h>

#include <time.h>//for the random function

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) printf("Socket Create Error\n");

    sockaddr_in info;
    sockaddr_in client_info;
    socklen_t client_addr_len;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = INADDR_ANY; // Local IP Address
    info.sin_port = htons(8080); // Port 8080

    int bind_result = bind(sockfd, (struct sockaddr*)&info, sizeof(info));
    if(bind_result < 0) printf("Bind Error\n");

    listen(sockfd, 1); // block until listen a client
    while(1){
        if(select(9,))
        client_fd = accept(sockfd, (struct sockaddr*)&client_info, &client_addr_len);
        printf("Accept client, client port: %d\n", client_info.sin_port);
    }

}
