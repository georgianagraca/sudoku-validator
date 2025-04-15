CC = gcc
CFLAGS = -Wall -pthread -fPIC
SRC = sudoku_validator.c
SO = sudoku_validator.so

all: $(SO)

$(SO): $(SRC)
	$(CC) $(CFLAGS) -shared -o $(SO) $(SRC)

clean:
	rm -f $(SO)
