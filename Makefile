CC = gcc
AR = ar
CFLAGS = -Wall

all: main server

main: main.o client.o
	${CC} ${CFLAGS} -o main main.o client.o

server: server.o
	${CC} ${CFLAGS} -o server server.o

main.o: main.c client.h main.h
	${CC} ${CFLAGS} -c main.c

server.o: server.c
	${CC} ${CFLAGS} -c server.c

client.o: client.c client.h
	${CC} ${CFLAGS} -c client.c

clean:
	rm -f *.a *.o main server