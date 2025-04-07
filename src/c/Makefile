CC = gcc
CFLAGS = -Wall -pthread
SRC = src/sudoku.c
OBJ = sudoku.o
BIN = sudoku-validator

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

sudoku.o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ) $(BIN)

run: all
	./$(BIN)
