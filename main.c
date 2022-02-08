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
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>  // Signal handling
#include "builtin.h"
#include "commandstruct.h"


// Global variable and signal handler for SIGTSTP signal
int fgOnly = 0;

void handler_SIGTSTP(int sig)
{
	if (fgOnly == 0)
	{
		write(1, "Entering foreground-only mode (& is now ignored)\n", 49);
		fgOnly++;
	}
	else
	{
		write(1, "Exiting foreground-only mode\n", 29);
		fgOnly--;
	}
}


int main(void)
{
	char* command;

	char* cmdStat = NULL;

	// Struct parameters to handle 'SIGTSTP' signal
	struct sigaction sigTstp;
	sigTstp.sa_handler = handler_SIGTSTP;
	sigTstp.sa_flags = SA_RESTART;

	do
	{
		command = commandPrompt();  // Command prompt funcition from builtinCmds.c file

		// Checks input for exit command
		if (strncmp(command, "exit", 4) != 0 && strncmp(command, "status", 6) != 0)
		{
			// Check for a blank line or a comment
			if (strncmp(command, "#", 1) != 0 && strcmp(command, "\n") != 0)
			{
				// Parse command information into a struct
				cmdStat = parseCommand(command, fgOnly);
			}
		}
		else if (strncmp(command, "status", 6) == 0)
		{
			// Check for a status message from the last child process
			if (cmdStat != NULL)
			{
				printf("%s", cmdStat);
				fflush(stdout);
			}
			else
			{
				printf("exit value 0");
				fflush(stdout);
			}

		}
		// Handle SIGTSTP signal in shell process
		sigaction(SIGTSTP, &sigTstp, NULL);

	} while (strncmp(command, "exit", 4) != 0);

	return 0;
}

