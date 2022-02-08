#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "cmd.h"  
#include "commandstruct.h"
#include "builtin.h"


// Function to handle background commands
int backgroundCmd(struct command* command)
{
	int procStatus;  // Holds the exit status of the waitpid function

	// Declare sigaction struct variable
	struct sigaction sigIgnore;
	sigIgnore.sa_handler = SIG_IGN;
	
	// Struct parameters to handle 'SIGTSTP' signal
	struct sigaction sigTstp;
	sigTstp.sa_handler = handler_SIGTSTP;
	sigTstp.sa_flags = SA_RESTART;

	char* usrInput;

	//struct information* 
	char* cmdStat = NULL;
	
	// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		perror("Error fork()\n");
		fflush(stdout);
		break;

	case 0:  //child process

		// Handle signals
		sigaction(SIGINT, &sigIgnore, NULL);
		sigaction(SIGTSTP, &sigIgnore, NULL);

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
			printf("%s: no such file or directory\n", command->cmd);
			fflush(stdout);
			exit(1);
		}

		break;

	default: // Parent process

		// Print message when background process gets started
		printf("Background PID is %ld\n", (long)spawnProc);
		fflush(stdout);

		long childProcess = (long) spawnProc;

		// Run command prompt while background process is running
		while ((spawnProc = waitpid(childProcess, &procStatus, WNOHANG)) == 0)
		{

			usrInput = commandPrompt();

			// Handle SIGTSTP signal in shell process
			sigaction(SIGTSTP, &sigTstp, NULL);

			// Checks input for exit command
			if (strncmp(usrInput, "exit", 4) != 0 && strncmp(usrInput, "status", 6) != 0)
			{
				// Check for a blank line or a comment
				if (strncmp(usrInput, "#", 1) != 0 && strcmp(usrInput, "\n") != 0)
				{
					// Parse command information into a struct
					cmdStat = parseCommand(usrInput, fgOnly);
				}
			}
			else if (strncmp(usrInput, "status", 6) == 0)
			{
				if (cmdStat != NULL)
				{
					printf("%s", cmdStat);
					fflush(stdout);
				}
				else
				{
					printf("exit status 0");
					fflush(stdout);
				}
			}
			else if (strncmp(usrInput, "exit", 4) == 0)
			{
				kill(childProcess, SIGTERM);  // Kills the child process
				exit(0);
			}
		}

		// Find and print the exit status of the child process
		if (WIFEXITED(procStatus))
		{
			printf("Background PID %ld is done. Exit Status: %d\n", childProcess, WEXITSTATUS(procStatus));
			fflush(stdout);
		}
		else if (WIFSIGNALED(procStatus))
		{
			printf("Background PID %ld is done. Termination signal: %d\n", childProcess, WTERMSIG(procStatus));
			fflush(stdout);
		}

		break;

	}

	return fgOnly;

}


// Function to run a foreground command
char* foregroundCmd(struct command* command)
{
	int procStatus;  // Holds the exit status of the waitpid function

	// Declare signal struct vars
	struct sigaction sigDefault;
	struct sigaction sigIgnore;
	sigDefault.sa_handler = SIG_DFL;
	sigIgnore.sa_handler = SIG_IGN;


	char* status = malloc(sizeof(char));

// Spawn new child process
	pid_t spawnProc = fork();

	switch (spawnProc)
	{
	case -1:  // Fork() error

		printf("Error fork()\n");
		fflush(stdout);
		break;

	case 0:  //child process

		// Handle signals
		sigaction(SIGINT, &sigDefault, NULL);
		sigaction(SIGTSTP, &sigIgnore, NULL);

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
			outputRedirect(command->outputFile);
		}

		// Run input command from a program in the PATH environment variable
		if (execvp(command->cmd, command->arguments) == -1)
		{
			printf("%s: no such file or directory\n", command->cmd);
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
			status = calloc(15, sizeof(char));
			sprintf(status, "exit value %d\n", WEXITSTATUS(procStatus));
		}
		else if (WIFSIGNALED(procStatus))
		{
			status = calloc(25, sizeof(char));
			sprintf(status, "terminated by signal %d\n", WTERMSIG(procStatus));
			printf("%s", status);
			fflush(stdout);
		}

		break;
	}

	return status;
}
