#ifndef CMD_H
#define CMD_H
#include "commandstruct.h"  // Connects to the 'struct command' definition

// Function to run non-built-in commands
void runCommand(struct command* newCommand);

// Function to redirect input
void inputRedirect(char* pathName);

// Function to redirect output
void outputRedirect(char* pathName);

#endif