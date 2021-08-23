CC=gcc
CFLAGS=-I. -std=c17
OBJDIR := dist
SRC := ./prompt.c ./history.c ./lash.c ./builtin.c ./input.c ./glob.c ./lib/string/strlib.c ./lib/time/time.c
OBJ := $(SRC:%.c=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.c
	mkdir -p '$(@D)'
	$(CC) -c $< $(CFLAGS) -o $@

lash: $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf dist lash