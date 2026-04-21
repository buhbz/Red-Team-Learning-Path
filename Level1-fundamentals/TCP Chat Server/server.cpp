#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <unistd.h>
#define Max 80
#define PORT 8080
#define SA struct sockaddr
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

void func(int connfd) {
    char buff[Max];
    int n;

    for (;;) {
        memset(buff, 0, Max);

        int bytesRecieved = recv(connfd, buff, Max,0);
        if (bytesRecieved > 0){break;}
        printf("From client: %s\t To Client: ", buff);

        memset(buff, 0, Max);
        n = 0;

        while ((buff[n++] = getchar()) != '\n');

        send(connfd, buff, Max,0);

        if (strncmp("exit", buff, 4) == 0) {
            cout << "\nexiting..." << endl;
            break;
        }


    }

}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), & wsaData);
    if (iResult != 0) {cout << "\n WSA startup failed."; exit(0);}

    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in address, client;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){cout << "socket creation failed."; exit(0);}
    cout << "\nSocket successfully created.";

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if ((bind(sockfd,(SA*)&address,sizeof(address))) != 0){cout << "\nbind failed."; exit(0);}
    else{cout << "\nbind successful.";}

    if ((listen(sockfd,5)) != 0){cout << "\nlisten failed."; exit(0);}
    else{cout << "\nlisten successful.";}

    len = sizeof(client);
    connfd = accept(sockfd, (SA*)&client, &len);
    if (connfd < 0){cout << "\nserver failed to accept connection."; exit(0);}
    else{cout << "\nserver accepted client.";}

    func(connfd);

    close(sockfd);
    WSACleanup();

    return 0;

}

