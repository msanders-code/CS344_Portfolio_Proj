#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmd.h"


// Redirects stdin to the given pathName
void inputRedirect(char* pathName)
{
	int fd;

	/* Attempts to open the file with 'pathName' path for read only.
	* If it fails, it prints an error message and sets the exit code to 1.
	* If it succeeds, it redirects stdin to the file that it just
	* opened.
	*/
	if ((fd = open(pathName, O_RDONLY)) == -1)
	{
		printf("cannot open %s for input\n", pathName);
		fflush(stdout);
		exit(1);
	}
	else
	{
		if (dup2(fd, 0) == -1)
		{
			perror("Error: input redirection\n");
			fflush(stdout);
			exit(1);
		}
		
		close(fd);  // Close original file descriptor
	}

}


// Redirects stdout to the given pathName
void outputRedirect(char* pathName)
{
	int fd;

	/* Attempts to open the file with 'pathName' path for writing only,
	* it creates a file if it does not exist or truncates an existing
	* file. If it fails, it prints an error message and sets the exit 
	* code to 1. If it succeeds, it redirects stdout to the file that 
	* it just opened.
	*/
	if ((fd = open(pathName, O_WRONLY | O_CREAT | O_TRUNC, 0660)) == -1)
	{
		printf("cannot open %s for output\n", pathName);
		fflush(stdout);
		exit(1);
	}
	else
	{
		if (dup2(fd, 1) == -1)
		{
			perror("Error: output redirection\n");
			fflush(stdout);
			exit(1);
		}
		
		close(fd);  // Close original file descriptor
	}

}