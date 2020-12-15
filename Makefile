# variables
CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -Iinclude
LDFLAGS = -lncurses
NAME = game

SRCS = $(shell find ./src -name "*.c")
OBJS = $(SRCS:c=o)

all: $(NAME) clean

$(NAME): $(OBJS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(OBJS) -o ./bin/$(NAME) $(LDFLAGS)

.o: .c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete