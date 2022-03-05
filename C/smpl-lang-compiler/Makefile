

CC=gcc
CFLAGS=-g -Wall -fsanitize=address
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HDRS=$(wildcard $(SRC)/*.h)
BINDIR=bin
BIN=$(BINDIR)/main

all: $(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean 
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -ri $(OBJ)/* $(BINDIR)/* #*~ *.dSYM
