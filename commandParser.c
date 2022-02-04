#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "commandstruct.h"
#include "builtin.h"
#include "cmd.h"


#define LEN 50 // Defines compound string length

/*
//for testing
void structPrint(struct command* newStruct)
{
	int index = 0;

	printf("Command: %s\n", newStruct->cmd);
	printf("Arguments: ");
	while (newStruct->arguments[index] != NULL)
	{
		printf("%s", newStruct->arguments[index]);

		if (newStruct->arguments[index + 1] != NULL)
		{
			printf(", ");
		}
		index++;
	}
	printf("\nInput File: %s\n", newStruct->inputFile);
	printf("Output File: %s\n", newStruct->outputFile);
	printf("Background: %s\n", newStruct->backGround);
	fflush(stdout);
}
//
*/

// Function to perform variable expansion
void argVarExpansion(char* argumentStr, struct command* structure, int index)
{
	// Get smallsh process PID to facilitate variable expansion for '$$' in command
	int processID = (int)getpid();

	// Allocate space
	char* expanded = malloc(sizeof(char));
	char* prefix = malloc(sizeof(char));

	char* secondPtr;
	int argSize;
	int numbDSigns;  // Number of '$' characters in command string
	char* subToken;

	argSize = strlen(argumentStr); // length of the input string

	if (strcmp(argumentStr, "$$") == 0)
	{
		expanded = calloc(LEN, sizeof(char));
		sprintf(expanded, "%d", processID);

		structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->arguments[index], expanded);
	}
	else
	{
		int count;

		if(strncmp(argumentStr, "$", 1) == 0)
		{
			count = (argSize - 2) / 2;

			prefix = calloc(LEN, sizeof(char));
			sprintf(prefix, "%d", processID);

			switch (argSize % 2)
			{
			case 0:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded) + 1, sizeof(char));
					strcpy(prefix, expanded);
				}

				structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
				strcpy(structure->arguments[index], expanded);
				break;

			case 1:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded) + 1, sizeof(char));
					strcpy(prefix, expanded);
				}

				expanded = calloc(LEN, sizeof(char));
				sprintf(expanded, "%s$", prefix);

				structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
				strcpy(structure->arguments[index], expanded);
				break;
			}
		}
		else
		{
			// Searches command for variable expansion characters
			subToken = strtok_r(argumentStr, "$", &secondPtr);

			prefix = calloc(strlen(subToken) + 1, sizeof(char));
			strcpy(prefix, subToken);

			numbDSigns = argSize - strlen(prefix);  // Calculates the number of expansions needed
			count = numbDSigns / 2;

			switch (numbDSigns % 2)
			{
			case 0:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded) + 1, sizeof(char));
					strcpy(prefix, expanded);
				}
				
				structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
				strcpy(structure->arguments[index], expanded);
				break;

			case 1:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded) + 1, sizeof(char));
					strcpy(prefix, expanded);
				}

				expanded = calloc(LEN, sizeof(char));
				sprintf(expanded, "%s$", prefix);

				structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
				strcpy(structure->arguments[index], expanded);
				break;
			}
		}
	}
	free(expanded);
	free(prefix);
}

// Function to perform variable expansion
void cmdVarExpansion(char* commandStr, struct command* structure, int index)
{
	// Get smallsh process PID to facilitate variable expansion for '$$' in command
	int processID = (int)getpid();

	// Allocate space
	char* expanded = malloc(sizeof(char));
	char* prefix = malloc(sizeof(char));

	char* secondPtr;
	int cmdSize;
	int numbDSigns;  // Number of '$' characters in command string

	cmdSize = strlen(commandStr);

	// Searches command for variable expansion characters
	char* subToken = strtok_r(commandStr, "$", &secondPtr);
	prefix = calloc(strlen(subToken) + 1, sizeof(char));
	strcpy(prefix, subToken);

	numbDSigns = cmdSize - strlen(prefix);  // Calculates the number of expansions needed
	int count = numbDSigns / 2;

	switch (numbDSigns % 2)
	{
	case 0:
		
		for (int i = 0; i < count; i++)
		{
			// Build variable expansion string
			expanded = calloc(LEN, sizeof(char));
			sprintf(expanded, "%s%d", prefix, processID);

			// Save the new string as the prefix
			prefix = calloc(strlen(expanded) + 1, sizeof(char));
			strcpy(prefix, expanded);
		}

		structure->cmd= calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->cmd, expanded);

		// Add command to argument array for use in a new process later
		structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->cmd, expanded);
		break;

	case 1:

		for (int i = 0; i < count; i++)
		{
			// Build variable expansion string
			expanded = calloc(LEN, sizeof(char));
			sprintf(expanded, "%s%d", prefix, processID);

			// Save the new string as the prefix
			prefix = calloc(strlen(expanded) + 1, sizeof(char));
			strcpy(prefix, expanded);
		}

		expanded = calloc(LEN, sizeof(char));
		sprintf(expanded, "%s$", prefix);

		structure->cmd = calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->cmd, expanded);

		// Add command to argument array for use in a new process later
		structure->arguments[index] = calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->cmd, expanded);
		break;
	}
	free(expanded);
	free(prefix);
}

// Function to parse the command line into a structure and call functions to execute the command
void parseCommand(char* command)
{
	// Allocate space
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

	/*Checks if entered command need variable expansion,
	performs the expansion,
	and saves the command in the structure
	*/
	if (strstr(token, "$$"))
	{
		cmdVarExpansion(token, newCommandLine, index);
	}
	else
	{
		// Adds the command to the structure if no var. expansion necessary
		newCommandLine->cmd = calloc(strlen(token) + 1, sizeof(char));
		strcpy(newCommandLine->cmd, token);

		// Adds the command to the frist position in the argument array to use in a new process later
		newCommandLine->arguments[index] = calloc(strlen(token) + 1, sizeof(char));
		strcpy(newCommandLine->arguments[index], token);
	}
	
	index++;

	while ((token = strtok_r(NULL, " \n", &savePtr)) != NULL)
	{
		if (strcmp(token, "&") == 0)
		{
			// Set the character to determine if command is running in the background
			newCommandLine->backGround = calloc(2, sizeof(char));
			strcpy(newCommandLine->backGround, token);
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

			if (strstr(token, "$$"))
			{
				argVarExpansion(token, newCommandLine, index);
			}
			else
			{
				// Add argument to argument array in structure
				newCommandLine->arguments[index] = calloc(strlen(token) + 1, sizeof(char));
				strcpy(newCommandLine->arguments[index], token);
			}
			index++;
		}
	}

	/*
	//test
	structPrint(newCommandLine);
	//
	*/
	
	// Run the parsed commands 
	if (strcmp(newCommandLine->cmd, "cd") == 0)
	{
		if (newCommandLine->arguments[1] == NULL)
		{
			changeDir("HOME");
		}
		else
		{
			changeDir(newCommandLine->arguments[1]);
		}
	}
	else
	{
		if (newCommandLine->backGround != NULL)
		{
			backgroundCmd(newCommandLine);
		}
		else
		{
			foregroundCmd(newCommandLine);
		}
	}

	// Free structure memory block
	free(newCommandLine);
}