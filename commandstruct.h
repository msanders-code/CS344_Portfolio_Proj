#ifndef COMMAND_STRUCT_H
#define COMMAND_STRUCT_H

// Structure definition to parse a command 
struct command
{
	char* cmd;
	char* arguments[514];
	char* inputFile;
	char* outputFile;
	char* backGround;
};

// Function to parse a user command line
char* parseCommand(char* command);

#endif