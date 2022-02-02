#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"

#define SIZE 4096

//testing
void printChDir(void)
{
	char* pathName = malloc(sizeof(char));
	pathName = calloc(SIZE, sizeof(char));

	// Get the current working directory
	if (getcwd(pathName, SIZE) == NULL)
	{
		printf("Error getcwd()");
	}
	else
	{
		printf("%s\n", pathName);
	}
	fflush(stdout);
	free(pathName);
}
//

// Changes the smallsh working directory
void changeDir(char* dirPath)
{
	char* homeDir;

	switch (strcmp(dirPath, "HOME"))
	{
	case 0:
		// change to directory specified in HOME env var

		if ((homeDir = getenv("HOME")) == NULL)
		{
			printf("Error get home directory");
			fflush(stdout);
		}

		// Change current working directory to HOME directory
		if (chdir(homeDir) != 0)
		{
			printf("Error chdir");
			fflush(stdout);
		}
		break;

	default:
		// Change current working directory to the directory specified as a command argument
		if (chdir(dirPath) != 0)
		{
			printf("Error chdir");
			fflush(stdout);
		}
		break;

	}
	

	//test
	printChDir();
	//
}
