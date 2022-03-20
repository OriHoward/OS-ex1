
#ifndef SYSTEM_CLIENT_H
#define SYSTEM_CLIENT_H

typedef struct Client {
    int sock;
    struct sockaddr_in *server;
    char message[1024];
} *pclient;

int initSocket();

pclient initClient();

int connectToServer();

int closeConn();

#endif //SYSTEM_CLIENT_H
