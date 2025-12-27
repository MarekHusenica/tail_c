CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -Werror=return-type
PROGRAM=tail

SRC_DIR=src
OBJS=tail.o circ_buffer.o

VENV=.venv
PYTEST=$(VENV)/bin/pytest

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

tail.o: $(SRC_DIR)/tail.c $(SRC_DIR)/circ_buffer.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/tail.c

circ_buffer.o: $(SRC_DIR)/circ_buffer.c $(SRC_DIR)/circ_buffer.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/circ_buffer.c


venv: $(VENV)/bin/activate

$(VENV)/bin/activate:
	python3 -m venv $(VENV)
	$(VENV)/bin/pip install pytest

test: all venv
	$(PYTEST) tests/

clean:
	rm -f *.o $(PROGRAM)
	rm -rf .pytest_cache
	find . -type d -name "__pycache__" -exec rm -rf {} +

distclean: clean
	rm -rf $(VENV)