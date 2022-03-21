
#ifndef SYSTEM_CLIENT_H
#define SYSTEM_CLIENT_H

typedef struct Client {
    int sock;
    char message[1024];
} *pclient;

int initSocket();

pclient initClient();

void closeConn(pclient client);

#endif //SYSTEM_CLIENT_H
