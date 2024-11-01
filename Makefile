CC=gcc
CFLAGS= -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS= -lncurses
OUT= out
SRC_DIR=.
SRCS = $(shell find $(SRC_DIR) -name '*.c' -a ! -path '*/tests/*')
OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(LDFLAGS) -o $(OUT) $(OBJS)

clean:
	$(RM) $(OUT) $(shell find . -name '*.o')

.PHONY: clean all

