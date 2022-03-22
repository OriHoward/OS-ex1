

#ifndef SYSTEM_SHELL_H
#define SYSTEM_SHELL_H

#define    MAX_SIZE_CMD    256
#define    MAX_SIZE_ARG    16

char cmd[MAX_SIZE_CMD];
char *argv[MAX_SIZE_ARG];
char cwd[256];
pid_t pid;
pclient client = NULL;
int tempDup = 300;
void getCmd();

void convertCmd();

void CShell();

int startsWith(const char *a, const char *b);

void handleDir();

void openTCP();

int handleCmd();

void handleCopy();

void copyFile(char *src, char *dst);

void handleDir();

#endif
