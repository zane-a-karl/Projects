

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

dry-run-clean:
	echo $(RM) -ri $(OBJ)/* $(BINDIR)/* $(SRC)/*~ hdr/*~ | tr -s ' ' | cut -d ' ' -f 1-100 | fmt -w 1 #*.dSYM

clean:
	$(RM) -r $(OBJ)/* $(BINDIR)/* $(SRC)/*~ hdr/*~ #*.dSYM
