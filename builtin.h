#ifndef BUILT_IN_H
#define BUILT_IN_H

// Changes the current working directory for smallsh
void changeDir(char* dirPath);

// Command prompt function
char* commandPrompt();

// variable and functions to deal with SIGTSTP signal
int fgOnly;
void handler_SIGTSTP(int sig);

#endif