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

int main(void)
{
	// Variables to hold the result of write and read system calls for error checking
	
	//int output = 0;  // Variable to keep the input loop going
	//ssize_t input;
	

	// Buffer to hold the user input
	char inputBuff[2049];
	char* usrInput = inputBuff;

	do
	{
		// Send ':' to terminal to serve as the command prompt and check for error
		//output = write(1, ": ", 2);
		if (write(1, ": ", 2) == -1)
		{
			printf("Error output");
			exit(1);
		}
		fflush(stdout);  // Sends data in stdout to the screen 

		// Read user input from terminal and check for error
		//input = read(0, &inputBuff, 2048);
		if (read(0, inputBuff, 2048) == -1)
		{
			printf("Error read");
			exit(1);
		}
		printf("%s", usrInput);
		//fflush(stdin);

		/*
		// Exit if the user enters exit
		if (strcmp(usrInput, "exit") == 0)
		{
			_exit(0);
			exit(0);
		}
		*/
	} while (strcmp(usrInput, "exit") != 0);  // End the input loop only if the user prompt has an error

	// Close stdin and stdout file descriptors before exiting program
	if (close(0) == -1)
	{
		printf("Error close stdin");
		exit(1);
	}

	if (close(1) == -1)
	{
		printf("Error clost stdout");
		exit(1)
	}

	exit(0);
}

