#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

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
//void log_handle(int sig);            // signal handler to add log statements
int startsWith(const char *a, const char *b);

int handleCmd();

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
        if (!strcmp("exit", cmd)) break;
        // handle commands
        if (handleCmd()) { continue; }

        // fit the command into *argv[]
        convertCmd();

        // fork and execute the command
        pid = fork();
        if (-1 == pid) {
            printf("failed to create a child\n");
        } else if (0 == pid) {
            // execute a command
            execvp(argv[0], argv);
        } else {
            // wait for the command to finish if "&" is not present
            if (NULL == argv[i]) waitpid(pid, NULL, 0);
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
    if (startsWith(cmd, "cd")) {
        char *to = cmd + 3;
        chdir(to);
        return 1;
    }

    return 0;
}

void getCmd() {
    // clause A :
//    printf("Shell>\t");
    // clause B - getting the current working directory path with getcwd function:
    if (getcwd(cwd, sizeof (cwd)) == NULL) {
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
        argv[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
    }
    //////// This array MUST be NULL terminated, i.e, the last element of argv must be a NULL pointer.

//    // check for "&"
//    if(!strcmp("&", argv[i-1])){
//        argv[i-1] = NULL;
//        argv[i] = "&";
//    }else{
//        argv[i] = NULL;
//    }
//    //printf("%d\n", i);
}

int startsWith(const char *a, const char *b) {
    if (strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

//void log_handle(int sig){
//    //printf("[LOG] child proccess terminated.\n");
//    FILE *pFile;
//    pFile = fopen("log.txt", "a");
//    if(pFile==NULL) perror("Error opening file.");
//    else fprintf(pFile, "[LOG] child proccess terminated.\n");
//    fclose(pFile);
//}
