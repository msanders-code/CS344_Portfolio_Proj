#ifndef COMMAND_STRUCT_H
#define COMMAND_STRUCT_H

// Structure definition to parse a command 
struct command
{
	char* cmd;
	char* arguments[512];
	char* inputFile;
	char* outputFile;
	char* backGround;
};

void parseCommand(char* command);

#endif