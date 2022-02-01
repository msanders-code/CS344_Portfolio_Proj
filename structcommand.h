#ifndef STRUCT_COMMAND_H
#define STRUCT_COMMAND_H

// Structure definition to parse a command 
struct command
{
	char* cmd;
	char* arguments[512];
	char* inputFile;
	char* outputFile;
	char* backGround;
};


#endif