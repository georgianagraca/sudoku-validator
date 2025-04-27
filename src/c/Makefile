CC = gcc
CFLAGS = -Wall -pthread -fPIC
LDFLAGS = -shared -Wl,-soname,sudoku_validator.so
SRC = sudoku_validator.c
TARGET = sudoku_validator.so  # Nome do binário executável

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o sudoku_validator.o
	$(CC) $(LDFLAGS) -o $(TARGET) sudoku_validator.o

clean:
	rm -f $(TARGET) *.o

