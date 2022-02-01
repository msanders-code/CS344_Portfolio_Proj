

setup:
	gcc -std=gnu99 -g -Wall -o smallsh main.c structBuilder.c

clean:
	rm smallsh
