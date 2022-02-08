#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "builtin.h"
#include "commandstruct.h"


// Changes the smallsh working directory
void changeDir(char* dirPath)
{
	char* homeDir;

	switch (strcmp(dirPath, "HOME"))
	{
	case 0:
		// Change current working directory to directory specified in HOME environment variable
		if ((homeDir = getenv("HOME")) == NULL)
		{
			printf("Error get home directory\n");
			fflush(stdout);
			exit(1);
		}

		if (chdir(homeDir) != 0)
		{
			printf("Error chdir\n");
			fflush(stdout);
			exit(1);
		}
		break;

	default:
		// Change current working directory to the directory specified as a command argument
		if (chdir(dirPath) != 0)
		{
			printf("Error chdir\n");
			fflush(stdout);
			exit(1);
		}
		break;

	}
}


// Command prompt for shell
char* commandPrompt()
{
	// Buffer to hold the user input
	char* usrInput = malloc(sizeof(char));

	// Struct parameters to handle 'SIGINT' signal
	struct sigaction sigInt;
	sigInt.sa_handler = SIG_IGN;

	// Handle SIGINT signal in shell process
	sigaction(SIGINT, &sigInt, NULL);

	// Send ':' to terminal to serve as the command prompt and check for error
	printf(": ");
	fflush(stdout);

	usrInput = calloc(2049, sizeof(char));

	// Read user input from terminal and check for error
	if (read(0, usrInput, 2048) == -1)
	{
		printf("Error read\n");
		fflush(stdout);
	}

	return usrInput;
}
