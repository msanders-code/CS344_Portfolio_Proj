#ifndef STRUCT_COMMAND_H
#define STRUCT_COMMAND_H

// Structure definition to parse a command 
struct command
{
	char* cmd;
	char* arguments;
	char* inputFile;
	char* outputFile;
	char* backGround;
};

struct command* parseCommand(char* command);

#endif