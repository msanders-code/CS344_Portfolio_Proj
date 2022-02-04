#ifndef CMD_H
#define CMD_H
#include "commandstruct.h"  // Connects to the 'struct command' definition

// Function to run non-built-in commands
void runCommand(struct command* newCommand);

// Function to redirect input
int inputRedirect(char* pathName);

// Function to redirect output
int outputRedirect(char* pathName);

#endif