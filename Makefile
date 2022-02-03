

setup:
	gcc -std=gnu99 -g -Wall -o smallsh main.c commandParser.c builtInCmds.c runCmd.c

clean:
	rm smallsh
