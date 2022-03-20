
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"

pclient initClient () {
    return (pclient)malloc(sizeof(struct Client));
}

int initSocket(pclient client) {
    client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock == -1) {
        printf("Could not create socket");
        return 0;
    }
    puts("Socket created");

    client->server->sin_addr.s_addr = inet_addr("127.0.0.1");
    client->server->sin_family = AF_INET;
    client->server->sin_port = htons(8888);
    return 1;
}

int connectToServer(pclient client) {
    if (connect(client->sock, (struct sockaddr *) &client->server, sizeof(client->server)) < 0) {
        perror("connect failed. Error");
        return 0;
    }
    puts("Connected\n");
    return 1;
}
