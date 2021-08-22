lsh: lsh.c
	$(CC) -o lsh builtin.c lsh.c history.c prompt.c input.c -Wall -Wextra -pedantic -std=c99
