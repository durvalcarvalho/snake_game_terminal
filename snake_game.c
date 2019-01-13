#include <stdio.h>
#include "snakelib.h"

int main()
{
	// setting the window config
	initscreen();

	int startx, starty = 0;
	getmaxyx(stdscr, starty, startx);

	// Creating a centralized window
	WINDOW* main_win = newwin(HEIGHT, WIDTH, starty/5, (startx-WIDTH)/2);

	// Enabling the arrows keys
	keypad(main_win, TRUE);

	// Printing a string in the window and refreshing
	char msg[] = "Use the arrow keys or 'ASDW' to play";
	mvprintw(1, (startx-strlen(msg))/2, "%s", msg);
	refresh();

	char** table;
	table = create_table(HEIGHT, WIDTH, '.');

	// init snake
	Snake snake = create_snake(HEIGHT, WIDTH);

	print_table(main_win, table, &snake);

	int pressedKey;
	
	Food_Position food = generate_food(table, foodIcon);

	while(TRUE)
	{
		mvprintw(3, (startx-9)/2, "Points: %d", points);

		if(isGameOver(&snake)) break;

		if(table[food.pos_y][food.pos_x] != foodIcon)
			food = generate_food(table, foodIcon);

		pressedKey = wgetch(main_win);

		halfdelay(5);

		switch(pressedKey)
		{
			case 83: // 'S'
			case 115: // 's'
			case KEY_DOWN:
				if(snake.direction != 'U')
					snake.direction = 'D';
				break;

			case 87: // 'W'
			case 119: // 'w'
			case KEY_UP:
				if(snake.direction != 'D')
					snake.direction = 'U';
				break;

			case 68: // 'D'
			case 100: // 'd'
			case KEY_RIGHT:
				if(snake.direction != 'L')
					snake.direction = 'R';
				break;

			case 65: // 'A'
			case 97: // 'a'
			case KEY_LEFT:
				if(snake.direction != 'R')
					snake.direction = 'L';
				break;

		}

		movement_snake(&snake, table);
		print_table(main_win, table, &snake);
		refresh();
	}

	endwin();

	for(int i=0; i<100; i++)
		printf("\n");

	printf("Game Over!\n");

	return 0;
}

