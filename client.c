
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"
#include <stdlib.h>

struct sockaddr_in server;

pclient initClient() {
    return (pclient) malloc(sizeof(struct Client));
}

void closeConn(pclient client) {
    close(client->sock);
    free(client);
}

int initSocket(pclient client) {
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
    puts("Connected");
    return 1;
}

