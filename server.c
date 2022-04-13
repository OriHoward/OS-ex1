
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main() {
    int sock, clientSock, c, read_size;
    struct sockaddr_in server, client;
    int running = 1;
    char clientMsg[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5555);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");


    listen(sock, 1);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while (running) {
        clientSock = accept(sock, (struct sockaddr *) &client, (socklen_t *) &c);
        if (clientSock < 0) {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");


        //Receive a message from client
        while ((read_size = recv(clientSock, clientMsg, sizeof(clientMsg), 0)) > 0) {
            if (!strcmp(clientMsg, "EXIT")) {
                running = 0;
                break;
            }
            clientMsg[sizeof(clientMsg) - 1] = '\0';
            puts(clientMsg);
            memset(clientMsg, 0, sizeof(clientMsg));
            clientMsg[sizeof(clientMsg) - 1] = '\0';

        }
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }

    return 0;
}

