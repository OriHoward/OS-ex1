
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include "server.h"


pserver initServer() {
    return (pserver) malloc(sizeof(struct Server));
}

int runServer(pserver serv) {
    serv->socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (serv->socket_desc == -1) {
        printf("Could not create socket\n");
    }
    puts("Socket created\n");

    serv->server->sin_family = AF_INET;
    serv->server->sin_addr.s_addr = INADDR_ANY;
    serv->server->sin_port = htons(8888);

    if (bind(serv->socket_desc, (struct sockaddr *) &serv->server, sizeof(serv->server)) < 0) {
        perror("bind failed. Error");
        return 0;
    }
    puts("bind done");

    listen(serv->socket_desc, 1);

    puts("Waiting for incoming connections...");
    serv->c = sizeof(struct sockaddr_in);

    serv->client_sock = accept(serv->socket_desc, (struct sockaddr *) &serv->client, (socklen_t *) &serv->c);
    if (serv->client_sock < 0) {
        perror("accept failed");
        return 0;
    }
    puts("Connection accepted");

    return 1;

}

//int main() {
//
//    //Receive a message from client
//    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
//        //Send the message back to client
//        write(client_sock, client_message, strlen(client_message));
//    }
//
//    if (read_size == 0) {
//        puts("Client disconnected");
//        fflush(stdout);
//    } else if (read_size == -1) {
//        perror("recv failed");
//    }
//
//    return 0;
//}