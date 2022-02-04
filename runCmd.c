#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cmd.h"  // Header file for holding declarations for the function to run non-built-in commands
#include "commandstruct.h"


// Function to handle background commands
void backgroundCmd(struct command* command)
{
	int procStatus;  // Holds the exit status of the waitpid function

	// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()");
		fflush(stdout);
		break;

	case 0:  //child process

		// Input redirection
		if (command->inputFile != NULL && command->outputFile != NULL)
		{
			inputRedirect(command->inputFile);
			outputRedirect(command->outputFile);
		}
		else if (command->inputFile != NULL && command->outputFile == NULL)
		{
			inputRedirect(command->inputFile);
			outputRedirect("/dev/null");
		}
		else if (command->inputFile == NULL && command->outputFile != NULL)
		{
			inputRedirect("/dev/null");
			outputRedirect(command->outputFile);
		}
		else
		{
			inputRedirect("/dev/null");
			outputRedirect("/dev/null");
		}

		// Run input command from a program in the PATH environment variable
		if (execvp(command->cmd, command->arguments) == -1)
		{
			printf("%s: Command could not be found!", command->cmd);
			fflush(stdout);
			exit(1);
		}

		break;

	default: // Parent process

		// Print message when background process gets started
		printf("Background PID is %ld\n", (long)spawnProc);
		fflush(stdout);

		// Wait for child process to terminate from running in the background
		spawnProc = waitpid(spawnProc, &procStatus, WNOHANG);
		break;

	}

	// Find and print the exit status of the child process
	if (WIFEXITED(procStatus))
	{
		printf("\nBackground PID %ld is done. Exit Status: %d\n", (long)spawnProc, WEXITSTATUS(procStatus));
		fflush(stdout);
	}
	else if (WIFSIGNALED(procStatus))
	{
		printf("\nBackground PID %ld is done. Termination signal: %d\n", (long)spawnProc, WTERMSIG(procStatus));
		fflush(stdout);
	}

}


// Function to run a foreground command
void foregroundCmd(struct command* command)
{
	int procStatus;  // Holds the exit status of the waitpid function

// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()");
		fflush(stdout);
		break;

	case 0:  //child process

		// Input redirection
		if (command->inputFile != NULL && command->outputFile != NULL)
		{
			inputRedirect(command->inputFile);
			outputRedirect(command->outputFile);
		}
		else if (command->inputFile != NULL && command->outputFile == NULL)
		{
			inputRedirect(command->inputFile);
		}
		else if (command->inputFile == NULL && command->outputFile != NULL)
		{
			inputRedirect("/dev/null");
		}

		// Run input command from a program in the PATH environment variable
		if (execvp(command->cmd, command->arguments) == -1)
		{
			printf("%s: Command could not be found!", command->cmd);
			fflush(stdout);
			exit(1);
		}

		break;

	default: // Parent process

		// Wait for child process to terminate from running in the foreground
		spawnProc = waitpid(spawnProc, &procStatus, 0);
		break;
	}
}
