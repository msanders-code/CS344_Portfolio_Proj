#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "commandstruct.h"


#define LEN 256  // Defines max compound string length
#define SUFFIX "$"  // Defines last part of string with odd number of '$' characters


//for testing
void structPrint(struct command* newStruct)
{
	int index = 0;

	printf("Command: %s\n", newStruct->cmd);
	fflush(stdout);
	printf("Arguments: ");
	fflush(stdout);
	while (newStruct->arguments[index] != NULL)
	{
		printf("%s, ", newStruct->arguments[index]);
		fflush(stdout);
		index++;
	}
	printf("\nInput File: %s\n", newStruct->inputFile);
	fflush(stdout);
	printf("Output File: %s\n", newStruct->outputFile);
	fflush(stdout);
	printf("Background: %s\n", newStruct->backGround);
	fflush(stdout);
}
//


// Function to perform variable expansion
void argVarExpansion(char* argumentStr, struct command* structure, int index)
{
	// Get smallsh process PID to facilitate variable expansion for '$$' in command
	//pid_t smallshPID = getpid();
	int processID = (int)getpid();

	// Allocate space
	char* expanded = malloc(sizeof(char));
	char* prefix = malloc(sizeof(char));

	char* secondPtr;
	int argSize;
	int numbDSigns;  // Number of '$' characters in command string
	char* subToken;

	if (strcmp(argumentStr, "$$") == 0)
	{
		expanded = calloc(LEN, sizeof(char));
		sprintf(expanded, "%d", processID);
		structure->arguments[index] = expanded;
	}
	else
	{

		argSize = strlen(argumentStr);
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
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				structure->arguments[index] = expanded;
				//strcpy(structure->arguments[index], expanded);
				break;

			case 1:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				expanded = calloc(LEN, sizeof(char));
				sprintf(expanded, "%s$", prefix);
				//strcat(expanded, SUFFIX);

				structure->arguments[index] = expanded;
				//strcpy(structure->arguments[index], expanded);
				break;
			}
		}
		else
		{
			// Searches command for variable expansion characters
			subToken = strtok_r(argumentStr, "$", &secondPtr);

			prefix = calloc(strlen(subToken) + 1, sizeof(char));
			strcpy(prefix, subToken);

			numbDSigns = argSize - strlen(subToken);  // Calculates the number of expansions needed
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
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				structure->arguments[index] = expanded;
				//strcpy(structure->arguments[index], expanded);
				break;

			case 1:

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				expanded = calloc(LEN, sizeof(char));
				sprintf(expanded, "%s$", prefix);

				structure->arguments[index] = expanded;
				//strcat(structure->arguments[index], SUFFIX);  // Adds the last '$' character to the command for an odd number of '$' characters
				break;
			}
		}
	}
	free(expanded);
	free(prefix);
}

// Function to perform variable expansion
void cmdVarExpansion(char* commandStr, struct command* structure)
{
	// Get smallsh process PID to facilitate variable expansion for '$$' in command
	//pid_t smallshPID = getpid();
	int processID = (int)getpid();

	// Allocate space
	char* expanded = malloc(sizeof(char));
	char* prefix = malloc(sizeof(char));

	char* secondPtr;
	int cmdSize;
	int numbDSigns;  // Number of '$' characters in command string

	cmdSize = strlen(commandStr);

	// Searches command for variable expansion characters
	char* subToken = strtok_r(commandStr, "$\0", &secondPtr);
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
			prefix = calloc(strlen(expanded), sizeof(char));
			strcpy(prefix, expanded);
		}

		structure->cmd= calloc(strlen(expanded), sizeof(char));
		strcpy(structure->cmd, expanded);
		break;

	case 1:

		for (int i = 0; i < count; i++)
		{
			// Build variable expansion string
			expanded = calloc(LEN, sizeof(char));
			sprintf(expanded, "%s%d", prefix, processID);

			// Save the new string as the prefix
			prefix = calloc(strlen(expanded), sizeof(char));
			strcpy(prefix, expanded);
		}

		structure->cmd = calloc(strlen(expanded) + 1, sizeof(char));
		strcpy(structure->cmd, expanded);
		strcat(structure->cmd, SUFFIX);  // Adds the last '$' character to the command for an odd number of '$' characters
		break;
	}
	free(expanded);
	free(prefix);
}

// Function to build a command structure
void parseCommand(char* command)
{
	// Allocate space
	struct command* newCommandLine = malloc(sizeof(struct command));
	
	/*
	char* expanded = malloc(sizeof(char));
	char* prefix = malloc(sizeof(char));
	*/

	/*
	// Get smallsh process PID to facilitate variable expansion for '$$' in command
	pid_t smallshPID = getpid();
	int processID = (int)smallshPID;
	*/

	// variable to move through the argument array
	int index = 0;

	// Initialize all struct members except for 'cmd' and 'arguments' to NULL
	newCommandLine->inputFile = NULL;
	newCommandLine->outputFile = NULL;
	newCommandLine->backGround = NULL;

	// Reentrant context pointer
	char* savePtr;
	//char* secondPtr;

	/*
	int cmdSize;
	int numbDSigns;  // Number of '$' characters in command string
	*/

	// Grabs command from input string
	char* token = strtok_r(command, " \n", &savePtr);

	/*Checks if entered command need variable expansion,
	performs the expansion,
	and saves the command in the structure
	*/
	if (strstr(token, "$$"))
	{
		cmdVarExpansion(token, newCommandLine);
			/*
			cmdSize = strlen(token);

			// Searches command for variable expansion characters
			char* subToken = strtok_r(token, "$\0", &secondPtr);
			prefix = calloc(strlen(subToken) + 1, sizeof(char));
			strcpy(prefix, subToken);

			numbDSigns = cmdSize - strlen(prefix);  // Calculates the number of expansions needed

			switch (numbDSigns % 2)
			{
			case 0:
				int count = numbDSigns / 2;

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				newCommandLine->cmd = calloc(strlen(expanded), sizeof(char));
				strcpy(newCommandLine->cmd, expanded);

				break;
			case 1:
				int count = numbDSigns / 2;

				for (int i = 0; i < count; i++)
				{
					// Build variable expansion string
					expanded = calloc(LEN, sizeof(char));
					sprintf(expanded, "%s%d", prefix, processID);

					// Save the new string as the prefix
					prefix = calloc(strlen(expanded), sizeof(char));
					strcpy(prefix, expanded);
				}

				newCommandLine->cmd = calloc(strlen(expanded) + 1, sizeof(char));
				strcpy(newCommandLine->cmd, expanded);
				strcat(newCommandLine->cmd, SUFFIX);  // Adds the last '$' character to the command for an odd number of '$' characters

				break;
			}
			*/
	
	}
	else
	{
		// Adds the command to the structure if no var. expansion necessary
		newCommandLine->cmd = calloc(strlen(token) + 1, sizeof(char));
		strcpy(newCommandLine->cmd, token);
	}

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
				newCommandLine->arguments[index] = token;
			}

			index++;
		}
	}

	//test
	structPrint(newCommandLine);
	//
	
	// Free structure memory block
	//free(newCommandLine);

}