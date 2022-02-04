#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cmd.h"  // Header file for holding declarations for the function to run non-built-in commands
#include "commandstruct.h"


void runCommand(struct command* newCommand)
{

	int procStatus;  // Holds the exit status of the waitpid function

	pid_t childPID;

	// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()");
		fflush(stdout);
		break;

	case 0:  //child process

		// Input redirction
		if (newCommand->inputFile != NULL && newCommand->outputFile != NULL)
		{
			inputRedirect(newCommand->inputFile);
			outputRedirect(newCommand->outputFile);
		}
		else if (newCommand->inputFile != NULL && newCommand->outputFile == NULL)
		{
			inputRedirect(newCommand->inputFile);

			if (newCommand->backGround != NULL)
			{
				outputRedirect("/dev/null");
			}

		}
		else if (newCommand->inputFile == NULL && newCommand->outputFile != NULL)
		{
			outputRedirect(newCommand->outputFile);

			if (newCommand->backGround != NULL)
			{
				inputRedirect("/dev/null");
			}
		}
		else if (newCommand->backGround != NULL)
		{
			inputRedirect("/dev/null");
			outputRedirect("/dev/null");
		}

		// Run input command from a program in the PATH environment variable
		execvp(newCommand->cmd, newCommand->arguments);

		break;

	default: // Parent process

		if (newCommand->backGround != NULL)
		{
			childPID = spawnProc;  // Copies chid PID

			// Print message when background process gets started
			printf("Background PID is %d\n", spawnProc);
			fflush(stdout);

			// Wait for child process to terminate from running in the background
			spawnProc = waitpid(spawnProc, &procStatus, WNOHANG);

			// Find and print the exit status of the child process
			if (WIFEXITED(procStatus))
			{
				printf("\nBackground PID %d is done. Exit Status: %d\n", childPID, WEXITSTATUS(procStatus));
				fflush(stdout);
			}
			else
			{
				printf("\nBackground PID %d is done. Termination signal: %d\n", childPID, WTERMSIG(procStatus));
				fflush(stdout);
			}

		}
		else
		{
			// Wait for child process to terminate from running in the forground
			spawnProc = waitpid(spawnProc, &procStatus, 0);
		}
		break;

	}
}