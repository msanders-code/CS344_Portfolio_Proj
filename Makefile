

setup:
	gcc -std=gnu99 -g -Wall -o smallsh main.c commandParser.c builtInCmds.c

clean:
	rm smallsh
