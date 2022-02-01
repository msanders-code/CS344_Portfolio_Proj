#include <stdlib.h>
#include <string.h>
#include "structcommand.h"

// Function to build a command structure
struct command* parseCommand(char* command)
{
	// Allocate space to store the new structure
	struct command* newCommandLine = malloc(sizeof(struct command));

	// variable to move through the argument array
	int index = 0;

	// Initialize all struct members except for 'cmd' and 'arguments' to NULL
	newCommandLine->inputFile = NULL;
	newCommandLine->outputFile = NULL;
	newCommandLine->backGround = NULL;

	// Reentrant context pointer
	char* savePtr;

	// Grabs command from input string
	char* token = strtok_r(command, " \n", &savePtr);
	newCommandLine->cmd = calloc(strlen(token) + 1, sizeof(char));
	strcpy(newCommandLine->cmd, token);

	while ((token = strtok_r(NULL, " \n", &savePtr)) != NULL)
	{
		if (strcmp(token, "&") == 0)
		{
			// Set the character to determine if command is running in the background
			*newCommandLine->backGround = '&';
		}
		else if (strcmp(token, "<") == 0)
		{
			// Grab file name from the string and add it to the struct
			token = strtok_r(NULL, " \n", &savePtr);
			newCommandLine->inputFile = calloc(strlen(token) + 1, sizeof(char));
			strcpy(newCommandLine->inputFile, token);
		}
		else if (strcmp(token, ">") == 0)
		{
			// Grab file name from the string and add it to the struct
			token = strtok_r(NULL, " \n", &savePtr);
			newCommandLine->outputFile = calloc(strlen(token) + 1, sizeof(char));
			strcpy(newCommandLine->outputFile, token);
		}
		else
		{
			// Add argument to argument array in structure
			newCommandLine->arguments[index] = token;
			index++;
		}
	}

	return newCommandLine;

}