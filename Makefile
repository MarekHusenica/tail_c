CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Werror=return-type
PROGRAM=tail

all: $(PROGRAM)

$(PROGRAM): tail.c
	$(CC) tail.c -o $(PROGRAM)

clean:
	rm -f *.o tail