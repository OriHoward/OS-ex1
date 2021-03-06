#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "client.h"
#include <dirent.h>
#include <limits.h>
#include "main.h"


/**
 * The main loop of the shell
 * The loop includes getCmd func, convertCmd func and the fork.
 */
void CShell() {
    while (1) {
        // get the command from user
        getCmd();
        // bypass empty commands
        if (!strcmp("", cmd)) continue;
        // check for "exit" command
        if (!strcmp("EXIT", cmd)) {
            if (isConnected) {
                printf("EXIT");
            }
            break;
        }
        // handle commands
        if (handleCmd()) { continue; }

        // fit the command into *argv[]
        convertCmd();


//      fork and execute the command
        pid = fork();
        if (-1 == pid) {
            printf("failed to create a child\n");
        } else if (0 == pid) {
            // execute a command
            if (execvp(argv[0], argv) == -1) {
                return;
            }
        } else {
            waitpid(pid, NULL, 0);
        }
    }
}

/**
 * this function handles the commands we get from the user that are necessary from the assigment.
 * @return
 */
int handleCmd() {
    // check if a sentence starts with ECHO and prints the echo
    if (startsWith(cmd, "ECHO")) {
        printf("%s \n", cmd + 5);
        return 1;
    }
    /**
     * checks if a sentence start with cd to change file
       The chdir is a system function.
     */

    if (startsWith(cmd, "cd")) {
        char *to = cmd + 3;
        chdir(to);
        return 1;
    }


    if (startsWith(cmd, "COPY")) {
        handleCopy(cmd + 5);
        return 1;
    }
    if (!strcmp("DIR", cmd)) {
        handleDir();
        return 1;
    }
    if (!strcmp("TCP PORT", cmd)) {
        openTCP();
        dup2(STDOUT_FILENO, tempDup);
        dup2(client->sock, STDOUT_FILENO);
        return 1;
    }
    /**
     * unlink is a system call
     */
    if (startsWith(cmd, "DELETE")) {
        char *toDelete = cmd + 7;
        unlink(toDelete);
        return 1;
    }
    if (!strcmp("LOCAL", cmd)) {
        if (isConnected) {
            isConnected = 0;
            closeConn(client);
            dup2(tempDup, STDOUT_FILENO);
        }
        return 1;
    }

    /**
     * if we get only one command we run it with system function - clause ??.
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

/**
 * This functions get the input command from the user.
 */
void getCmd() {
    // clause A :
//    printf("Yes master?\t");
    // clause B - getting the current working directory path with getcwd function:
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    } else {
        printf("<%s>", cwd);
    }
    // get the command from the user
    fgets(cmd, MAX_SIZE_CMD, stdin);
    if ((strlen(cmd) > 0) && (cmd[strlen(cmd) - 1] == '\n'))
        cmd[strlen(cmd) - 1] = '\0';
}

/**
 * Splits the string command that we get from the user to an array of strings.
 */
void convertCmd() {
    // split string into argv
    char *ptr;
    int i = 0;
    ptr = strtok(cmd, " ");
    while (ptr != NULL) {
        argv[i++] = ptr;
        ptr = strtok(NULL, " ");
    }
}

/**
 * Check if a string starts with a specific substring
 * @param a
 * @param b
 * @return
 */
int startsWith(const char *a, const char *b) {
    if (strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

/**
 * handle the Copy file to another file
 * @param string
 */
void handleCopy(char *string) {
    char src[50];
    char dst[50];
    int index = 0;
    int dstInd = 0;
    while (string[index] != ' ') {
        src[index] = string[index];
        index++;
    }
    index++;
    src[index] = '\0';
    for (int i = index; i < strlen(string); ++i) {
        dst[dstInd] = string[i];
        dstInd++;
    }
    dst[dstInd] = '\0';
    copyFile(src, dst);
}

/**
 * the fopen,fread,fwrite are library functions.
 */

void copyFile(char *src, char *dst) {
    char c;
    FILE *srcFile = fopen(src, "rb");
    if (srcFile == NULL) {
        printf("Cannot open file %s\n", src);
        return;
    }
    FILE *dstFile = fopen(dst, "wb");
    if (dstFile == NULL) {
        printf("Cannot open file %s\n", dst);
        return;
    }
    while (fread(&c, 1, 1, srcFile) == 1) {
        fwrite(&c, 1, 1, dstFile);
    }
    fclose(srcFile);
    fclose(dstFile);
}

/**
 * opens a TCP connection (with client and the server)
 */
void openTCP() {
    client = initClient();
    initSocket(client);
    isConnected = 1;
}

/**
 * Dir func - shows all the files in the current directory
 */
void handleDir() {
    DIR *dr;
    struct dirent *dir;
    dr = opendir(".");
    if (dr) {
        while ((dir = readdir(dr)) != NULL) {
            if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
                printf("%s\n", dir->d_name);
            }
        }
        closedir(dr);
    }
    return;
}

/**
 * the main to run the shell
 * @return
 */
int main() {
    CShell();
    return 0;
}
