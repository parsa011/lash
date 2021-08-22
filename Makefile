CC=gcc
CFLAGS=-I.
DEPS = lash.h builtin.h prompt.h history.h input.h ./lib/string/strlib.h
OBJ = lash.o builtin.o prompt.o history.o input.o ./lib/string/strlib.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lash: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f *.o 
