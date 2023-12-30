CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
LDLIBS=-lncurses

nlife: nlife.c
	$(CC) $(CFLAGS) -o nlife nlife.c $(LDLIBS)

clean:
	rm nlife
