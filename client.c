
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"


pclient initClient() {
    return (pclient) malloc(sizeof(struct Client));
}

void closeConn(pclient client) {
    free(client);
    close(client->sock);
}

int initSocket(pclient client) {
    struct sockaddr_in server;
    client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock == -1) {
        printf("Could not create socket");
        return 0;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    if (connect(client->sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 0;
    }
    puts("Connected\n");
    return 1;
}

