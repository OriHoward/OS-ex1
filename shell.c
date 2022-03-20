#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "client.h"
#include "server.h"

#define    MAX_SIZE_CMD    256
#define    MAX_SIZE_ARG    16


char cmd[MAX_SIZE_CMD];                // string holder for the command
char *argv[MAX_SIZE_ARG];            // an array for command and arguments
char cwd[256];
pid_t pid;                                        // global variable for the child process ID
char i;                                                // global for loop counter

void getCmd();                                // get command string from the user
void convertCmd();                        // convert the command string to the required format by execvp()
void CShell();                                // to start the shell
int startsWith(const char *a, const char *b);

void openTCP();

int handleCmd();

void handleCopy();

void handleDir();

int main() {

    // tie the handler to the SGNCHLD signal

//    signal(SIGCHLD, log_handle);
//
//// I changed it to NULL - need to check
//    signal(SIGCHLD, NULL);

    // start the shell
    CShell();

    return 0;
}

void CShell() {
    while (1) {
        // get the command from user
        getCmd();
        // bypass empty commands
        if (!strcmp("", cmd)) continue;
        // check for "exit" command
        if (!strcmp("EXIT", cmd)) break;
        // handle commands
        if (handleCmd()) { continue; }

        // fit the command into *argv[]
        convertCmd();


//         fork and execute the command
        pid = fork();
        if (-1 == pid) {
            printf("failed to create a child\n");
        } else if (0 == pid) {
            // execute a command
            if (execvp(argv[0], argv) == -1) {
                break;
            }
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}

int handleCmd() {
    // check if a sentence starts with ECHO and prints the echo
    if (startsWith(cmd, "ECHO")) {
        printf("%s \n", cmd + 5);
        return 1;
    }
    // check if a sentence start with cd to change file
    // the chdir is a system function.
    if (startsWith(cmd, "cd")) {
        char *to = cmd + 3;
        chdir(to);
        return 1;
    }
    // the fopen,fread,fwrite are library functions.

    if (startsWith(cmd, "COPY")) {
        handleCopy();
    }
    if (!strcmp("DIR", cmd)) {
        handleDir();
    }
    if (!strcmp("abc", cmd)) {
        printf("aaaa\n");
        openTCP();
    }

    /**
     * if we get only one command we run it with system function - clause ח.
     * The system() is a library function that uses fork(2) to create a child
       process that executes the shell command specified in command
       using execl(3) as follows:
           execl("/bin/sh", "sh", "-c", command, (char *) NULL);
       system() returns after the command has been completed.
       source - linux man page
     */
//    if (!strchr(cmd,' ')) {
//        system(cmd);
//        return 1;
//    }


    return 0;
}

void getCmd() {
    // clause A :
//    printf("Yes master?\t");
    // clause B - getting the current working directory path with getcwd function:
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    } else {
        printf("<%s>", cwd);
    }

    // get command from user
    fgets(cmd, MAX_SIZE_CMD, stdin);
    // remove trailing newline
    if ((strlen(cmd) > 0) && (cmd[strlen(cmd) - 1] == '\n'))
        cmd[strlen(cmd) - 1] = '\0';
}

void convertCmd() {
    // split string into argv
    char *ptr;
    i = 0;
    ptr = strtok(cmd, " ");
    while (ptr != NULL) {
//        printf("%s\n", ptr);
        argv[i++] = ptr;
        ptr = strtok(NULL, " ");
    }

}

int startsWith(const char *a, const char *b) {
    if (strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

void handleCopy() {
    printf("empty");
}

void handleDir() {
    printf("dir");
}

void openTCP() {
    pclient client = NULL;
    client = initClient();
    printf("arrived here\n");
    initSocket(client);
    printf("connected");

}