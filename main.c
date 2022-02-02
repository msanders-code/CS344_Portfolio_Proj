/*
* Author: 
*	Matt Sanders
* 
* Description:
* 
*	This program creates an instance of a shell
*	and allows the user to execute a number of
*	standard shell functionality. It is written
*	in C, designed for use in a LINUX based system.
*/

#include <unistd.h>  // Various functions pertaining to the LINUX system
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "commandstruct.h"


int main(void)
{

	// Buffer to hold the user input
	char* usrInput = malloc(sizeof(char));

	do
	{
		// Send ':' to terminal to serve as the command prompt and check for error
		if (write(1, ": ", 2) == -1)
		{
			printf("Error output");
			fflush(stdout);
			exit(1);
		}

		usrInput = calloc(2049, sizeof(char));

		// Read user input from terminal and check for error
		if (read(0, usrInput, 2048) == -1)
		{
			printf("Error read");
			fflush(stdout);
			exit(1);
		}

		// Checks input for exit command
		if (strcmp(usrInput, "exit\n") != 0 && strcmp(usrInput, "\n") != 0)
		{
			// Check for a blank line or a comment
			if (strncmp(usrInput, "#", 1) != 0)
			{
				// Parse command information into a struct
				parseCommand(usrInput);
			}
		}

	} while (strcmp(usrInput, "exit\n") != 0);  // End the input loop only if the user prompt has an error

	// Close stdin and stdout file descriptors before exiting program
	if (close(0) == -1)
	{
		printf("Error close stdin");
		fflush(stdout);
		exit(1);
	}

	if (close(1) == -1)
	{
		printf("Error close stdout");
		fflush(stdout);
		exit(1);
	}

	exit(0);
}

