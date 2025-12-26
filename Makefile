CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Werror=return-type
PROGRAM=tail
OBJS=tail.o circ_buffer.o

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

tail.o: tail.c circ_buffer.h
	$(CC) $(CFLAGS) -c tail.c

circ_buffer.o: circ_buffer.c circ_buffer.h
	$(CC) $(CFLAGS) -c circ_buffer.c

clean:
	rm -f *.o tail