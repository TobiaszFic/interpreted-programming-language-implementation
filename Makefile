UNAME_S = $(shell uname -s)

CC = clang 
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wstrict-aliasing
LDFLAGS =-lm

SRC  = $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: game

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/exe $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ)
