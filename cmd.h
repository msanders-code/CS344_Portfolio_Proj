#ifndef CMD_H
#define CMD_H
#include "commandstruct.h"  // Connects to the 'struct command' definition

// Function to run non-built-in background commands
void backgroundCmd(struct command* command);

// Function to run non-built-in foreground commands
char* foregroundCmd(struct command* command);

// Function to redirect input
void inputRedirect(char* pathName);

// Function to redirect output
void outputRedirect(char* pathName);

#endif