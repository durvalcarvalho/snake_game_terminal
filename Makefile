CC := g++

CFLAGS := -W -Wall -Wextra -Wshadow -pedantic -lncurses

# SRCFILES transforma o .cpp em .o para a pasta obj
all:
	$(CC) snake_game.c snakelib.h -lncurses -o SnakeGame

run:
	./SnakeGame