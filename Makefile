CC=clang
CFLAGS=-O2 -Wall -Wextra -pedantic -std=c11
LINKER_FLAGS = -lpq
TARGET=pgtest
INCLUDE_PATHS= -I/usr/include/postgresql

SRCS=main.c pgclient.c users.c

all : $(SRCS)
	$(CC) $(SRCS) $(INCLUDE_PATHS) $(CFLAGS) $(LINKER_FLAGS) -o $(TARGET)