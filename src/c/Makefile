CC = gcc
CFLAGS = -Wall -pthread
SRC = sudoku_validator.c
OBJ = sudoku_validator.o
BIN = sudoku_validator

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

sudoku.o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ) $(BIN)

run: all
	./$(BIN)
