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
	
	char* usrInput = malloc(sizeof(char));
	
	// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()\n");
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
			printf("%s: Command could not be found!\n", command->cmd);
			fflush(stdout);
			exit(1);
		}

		break;

	default: // Parent process

		// Print message when background process gets started
		printf("Background PID is %ld\n", (long)spawnProc);
		fflush(stdout);

		int childProcess = spawnProc;

		while ((spawnProc = waitpid(childProcess, &procStatus, WNOHANG)) == 0)
		{

			//Run command prompt
			if (write(1, ": ", 2) == -1)  // Send ':' to terminal to serve as the command prompt and check for error
			{
				printf("Error output\n");
				fflush(stdout);
				exit(1);
			}

			usrInput = calloc(2049, sizeof(char));

			// Read user input from terminal and check for error
			if (read(0, usrInput, 2048) == -1)
			{
				printf("Error read\n");
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
			else if (strcmp(usrInput, "exit\n") == 0)
			{
				exit(0);
			}
		}

		// Find and print the exit status of the child process
		if (WIFEXITED(procStatus))
		{
			printf("Background PID %ld is done. Exit Status: %d\n", (long)spawnProc, WEXITSTATUS(procStatus));
			fflush(stdout);
		}
		else if (WIFSIGNALED(procStatus))
		{
			printf("Background PID %ld is done. Termination signal: %d\n", (long)spawnProc, WTERMSIG(procStatus));
			fflush(stdout);
		}

		break;

	}

	free(usrInput);
}


// Function to run a foreground command
char* foregroundCmd(struct command* command)
{
	int procStatus;  // Holds the exit status of the waitpid function

	char* status = malloc(sizeof(char));

// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()\n");
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
			printf("%s: Command could not be found!\n", command->cmd);
			fflush(stdout);
			exit(1);
		}

		break;

	default: // Parent process

		// Wait for child process to terminate from running in the foreground
		spawnProc = waitpid(spawnProc, &procStatus, 0);

		// Find and print the exit status of the child process
		if (WIFEXITED(procStatus))
		{
			status = calloc(13, sizeof(char));
			sprintf(status, "exit value %d\n", procStatus);
		}
		else if (WIFSIGNALED(procStatus))
		{
			status = calloc(30, sizeof(char));
			sprintf(status, "terminated by signal %d\n", procStatus);
		}

		break;
	}

	return status;
}
