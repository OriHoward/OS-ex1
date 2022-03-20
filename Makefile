CC = gcc
AR = ar
CFLAGS = -Wall

all: shell

shell: shell.o client.o
	${CC} ${CFLAGS} -o shell shell.o client.o

server: server.o
	${CC} ${CFLAGS} -o server server.o

shell.o: shell.c client.h
	${CC} ${CFLAGS} -c shell.c

server.o: server.c server.h
	${CC} ${CFLAGS} -c server.c

client.o: client.c client.h
	${CC} ${CFLAGS} -c client.c

clean:
	rm -f *.a *.o shell