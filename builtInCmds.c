#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"


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
			printf("Error get home directory");
			fflush(stdout);
		}

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
}
