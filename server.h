
#ifndef SYSTEM_SERVER_H
#define SYSTEM_SERVER_H

typedef struct Server {
    int socket_desc, client_sock, c, read_size;
    char client_message[2000];
} *pserver;

int runServer();

pserver initServer();

#endif //SYSTEM_SERVER_H
