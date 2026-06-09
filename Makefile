CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror -std=c11 -O3

PREFIX ?= /usr/local
INCLUDEDIR := $(PREFIX)/include
LIBDIR := $(PREFIX)/lib

NAME := tinygcp
LIB := lib$(NAME).a
SRC := tinygcp.c
OBJ := tinygcp.o

all: $(LIB)

$(OBJ): $(SRC) tinygcp.h
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

install: $(LIB)
	install -d $(INCLUDEDIR)
	install -d $(LIBDIR)
	install -m 644 tinygcp.h $(INCLUDEDIR)/
	install -m 644 $(LIB) $(LIBDIR)/
	$(MAKE) clean

uninstall:
	rm -f $(INCLUDEDIR)/tinygcp.h
	rm -f $(LIBDIR)/$(LIB)

clean:
	rm -f $(OBJ) $(LIB)

.PHONY: all install uninstall clean
