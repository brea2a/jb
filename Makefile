
CFLAGS += -Wall -Werror

all: jo jo.1 jo.md

jo: jo.c json.o
	$(CC) $(CFLAGS) -o jo jo.c json.o

json.o: json.c json.h

jo.1: jo.pandoc
	-pandoc -s -w man jo.pandoc -o jo.1

jo.md: jo.pandoc
	-pandoc -w markdown jo.pandoc -o jo.md

clean:
	rm -f *.o
clobber: clean
	rm -f jo
