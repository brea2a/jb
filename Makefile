
CFLAGS=-Wall -Werror

all: jo jo.1 README.md

jo: jo.c json.o
	$(CC) $(CFLAGS) -o jo jo.c json.o

json.o: json.c json.h

jo.1: jo.pandoc
	pandoc -s -w man jo.pandoc -o jo.1

README.md: jo.pandoc
	pandoc -w markdown jo.pandoc -o README.md

clean:
	rm -f *.o
clobber: clean
	rm -f jo
