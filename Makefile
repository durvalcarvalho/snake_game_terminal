CC := gcc

CFLAGS := -W -Wall -Wextra -Wshadow -pedantic -lncurses

# SRCFILES transforma o .cpp em .o para a pasta obj
all:
	$(CC) $(CFLAGS) snake_game.c -o SnakeGame

run:
	./SnakeGame