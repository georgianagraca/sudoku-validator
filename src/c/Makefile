CC = gcc
CFLAGS = -Wall -pthread -fPIC
TARGETS = get_sudoku.so sudoku_validator_1thread.so sudoku_validator_9thread.so

all: $(TARGETS)

get_sudoku.so: get_sudoku.c
	$(CC) $(CFLAGS) -shared -o get_sudoku.so get_sudoku.c

sudoku_validator_1thread.so: sudoku_validator_1thread.c
	$(CC) $(CFLAGS) -shared -o sudoku_validator_1thread.so sudoku_validator_1thread.c

sudoku_validator_9thread.so: sudoku_validator_9thread.c
	$(CC) $(CFLAGS) -shared -o sudoku_validator_9thread.so sudoku_validator_9thread.c

clean:
	rm -f $(TARGETS)
