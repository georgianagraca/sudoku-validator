CC = gcc
CFLAGS = -Wall -pthread
SRC = sudoku_validator.c
OBJ = sudoku_validator.o
BIN = sudoku_validator

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)

run: all
	./$(BIN)
