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
		
		// Input and Output redirection vars that hold the fd's to be able to close them when the process ends
		int rdIn = 0;
		int wrOut = 0;

		if (newCommand->inputFile != NULL && newCommand->outputFile != NULL)
		{
			rdIn = inputRedirect(newCommand->inputFile);
			wrOut = outputRedirect(newCommand->outputFile);
		}
		else if (newCommand->inputFile != NULL && newCommand->outputFile == NULL)
		{
			rdIn = inputRedirect(newCommand->inputFile);

			if (newCommand->backGround != NULL)
			{
				wrOut = outputRedirect("/dev/null");
			}

		}
		else if (newCommand->inputFile == NULL && newCommand->outputFile != NULL)
		{
			wrOut = outputRedirect(newCommand->outputFile);

			if (newCommand->backGround != NULL)
			{
				rdIn = inputRedirect("/dev/null");
			}
		}
		else if (newCommand->backGround != NULL)
		{
			rdIn = inputRedirect("/dev/null");
			wrOut = outputRedirect("/dev/null");
		}

		execvp(newCommand->cmd, newCommand->arguments);
		
		/*
		if (close(rdIn) == -1)
		{
			perror("Error close input fd %d", rdIn);
			exit(1);
		}

		if (close(wrOut) == -1)
		{
			perror("Error close output fd %d", wrOut);
			exit(1)
		}
		*/

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