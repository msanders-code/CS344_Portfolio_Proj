#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmd.h"


// Redirects stdin to the given pathName
int inputRedirect(char* pathName)
{
	int fd;
	int rdNewfd;

	/* Attempts to open the file with 'pathName' path for read only.
	* If it fails, it prints an error message and sets the exit code to 1.
	* If it succeeds, it redirects stdin to the file that it just
	* opened.
	*/
	if ((fd = open(pathName, O_RDONLY)) == -1)
	{
		perror("Error: Could not open file for reading");
		fflush(stdout);
		exit(1);
	}
	else
	{
		if ((rdNewfd = dup2(fd, 0)) == -1)
		{
			perror("Error: input redirection");
			fflush(stdout);
			exit(1);
		}
		
		close(fd);  // Close original file descriptor
	}

	return rdNewfd;  // Return new file descriptor
}


// Redirects stdout to the given pathName
void outputRedirect(char* pathName)
{
	int fd;
	int wrNewfd;

	/* Attempts to open the file with 'pathName' path for writing only,
	* it creates a file if it does not exist or truncates an existing
	* file. If it fails, it prints an error message and sets the exit 
	* code to 1. If it succeeds, it redirects stdout to the file that 
	* it just opened.
	*/
	if ((fd = open(pathName, O_WRONLY | O_CREAT | O_TRUNC)) == -1)
	{
		perror("Error: Could not open file for writing");
		fflush(stdout);
		exit(1);
	}
	else
	{
		if ((wrNewfd = dup2(fd, 1)) == -1)
		{
			perror("Error: output redirection");
			fflush(stdout);
			exit(1);
		}
		
		close(fd);  // Close original file descriptor
	}

	return wrNewfd;  // Return new file descriptor
}