CC = gcc
AR = ar
CFLAGS = -Wall

all: main shell
main: main.o
	${CC} ${CFLAGS} -o main main.o

shell: shell.o
	${CC} ${CFLAGS} -o shell shell.o

main.o: main.c
	${CC} ${CFLAGS} -c main.c

shell.o: shell.c
	${CC} ${CFLAGS} -c shell.c

clean:
	rm -f *.a *.o main shell