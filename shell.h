

#ifndef SYSTEM_SHELL_H
#define SYSTEM_SHELL_H

#define    MAX_SIZE_CMD    256
#define    MAX_SIZE_ARG    16

char cmd[MAX_SIZE_CMD];                // string holder for the command
char *argv[MAX_SIZE_ARG];            // an array for command and arguments
char cwd[256];
pid_t pid;                                        // global variable for the child process ID
char i;                                                // global for loop counter
pclient client = NULL;

void getCmd();                                // get command string from the user
void convertCmd();                        // convert the command string to the required format by execvp()
void CShell();                                // to start the shell
int startsWith(const char *a, const char *b);

void handleDir();

void openTCP();

int handleCmd();

void handleCopy();

void handleDir();

#endif //SYSTEM_SHELL_H
