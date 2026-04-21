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

void func(int sockfd) {
    char buff[Max];
    int n;
    for (;;) {
        memset(buff, 0, Max);
        cout << "Please enter a string" << endl;

        while ((buff[n++] = getchar()) != '\n');
        send(sockfd, buff, Max, 0);

        memset(buff, 0, Max);
        recv(sockfd, buff, Max, 0);

        printf("From server: %s", buff);
        if ((strncmp(buff, "exit", 4) == 0) || (strncmp(buff, "quit", 4) == 0)) {
            printf("Client Exit...");
            break;
        }
    }
}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {cout << "\n WSA Startup Failed."; exit(0);}

    int sockfd,connfd;
    struct sockaddr_in address, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {cout << "\n Socket Creation Failed."; exit(0);}
    cout << "\nSocket Successfully Created.";

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&address, sizeof(address)) != -1) {cout << "\n Connection Failed."; exit(0);}
    cout << "\n Connection Successful.";

    func(sockfd);
    closesocket(sockfd);

    return 0;

}