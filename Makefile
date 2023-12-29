LDLIBS=-lncurses

build: nlife.c
	$(CC) -o nlife nlife.c $(LDLIBS)

clean:
	rm nlife
