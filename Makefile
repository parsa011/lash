lash: lash.c
	$(CC) -o lash builtin.c lash.c history.c prompt.c input.c -Wall -Wextra -pedantic -std=c99
