CC = gcc
CFLAGS = -Wall -pthread -fPIC
SRC = sudoku_validator.c
BIN = sudoku_validator  # Nome do binário executável

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)  # Gera o binário executável

clean:
	rm -f $(BIN)
