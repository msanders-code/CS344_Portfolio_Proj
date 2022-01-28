/*
* Author: 
*	Matt Sanders
* 
* Description:
* 
*	This program create an instance of a shell
*	and allows the user to execute a number of
*	standard shell functionality. It is written
*	in C, designed for use in a LINUX based system.
*/

#include <unistd.h>  // Various functions pertaining to the LINUX system
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	// Variables to hold the result of write and read system calls for error checking
	ssize_t output;
	ssize_t input;

	// Buffer to hold the user input
	char inputBuff[2049];

	do
	{
		// Send ':' to terminal to serve as the command prompt and check for error
		output = write(1, ": ", 2);
		if (output == -1)
		{
			printf("Error output");
			exit(1);
		}

		// Read user input from terminal and check for error
		input = read(0, &inputBuff, 2048);
		if (input == -1)
		{
			printf("Error read");
			exit(1);
		}

		// Exit if the user enters exit
		if (strcmp(&inputBuff, "exit") == 0)
		{
			exit(0);
		}

	} while (output != -1);  // End the input loop only if the user prompt has an error

	exit(1);
}

