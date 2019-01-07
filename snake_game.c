#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

// MACROS
#define WIDTH 50
#define HEIGHT 20


// STRUCTURES
typedef struct _snake_body
{
	int pos_x;
	int pos_y;
	char body; // '>', '<', '^', 'V'
	
} Snake_Body;

typedef struct _snake
{
	char direction; // D, U, R, L (Down, Up, Right, Left)
	int size;
	Snake_Body* Body;
} Snake;


// FUNCTIONS
void print_table(WINDOW*, char**, Snake*);
char** create_table(int, int, char);
Snake create_snake(int, int);
void insert_snake(char**, Snake*);
bool isGameOver(Snake*);
void movement_snake(Snake*);

int main()
{
	initscr();
	clear();
	noecho();
	// cbreak(); // Line buffering disabled, pass on everything
	curs_set(0); // disable blink cursor

	int startx, starty = 0;
	getmaxyx(stdscr, starty, startx);

	// Creating a main window and centering
	WINDOW* main_win = newwin(HEIGHT, WIDTH, starty/5, (startx-WIDTH)/2);

	// geting arrow keys
	keypad(main_win, TRUE);

	char msg[] = "Use as setas ou 'ASDW' para jogar";
	mvprintw(1, (startx-strlen(msg))/2, "%s", msg);
	refresh();

	char** table;
	table = create_table(HEIGHT, WIDTH, '.');

	// init snake
	Snake snake = create_snake(HEIGHT, WIDTH);

	print_table(main_win, table, &snake);

	int c;
	while(TRUE)
	{
		if(isGameOver(&snake)) break;

		c = wgetch(main_win);

		halfdelay(1);

		switch(c)
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

		movement_snake(&snake);

		print_table(main_win, table, &snake);

		refresh();
	}

	// clrtoeol();
	refresh();
	endwin();

	printf("Game Over!\n");

	return 0;
}

void movement_snake(Snake* snake)
{
	int delta_y=0, delta_x=0;

	switch(snake->direction)
	{
		case 'D':
			snake->Body[0].body = 'V';
			delta_y = +1;
		break;

		case 'U':
			snake->Body[0].body = '^';
			delta_y = -1;
			break;

		case 'R':
			snake->Body[0].body = '>';
			delta_x = 1;
			break;

		case 'L':
			snake->Body[0].body = '<';
			delta_x = -1;
			break;
	}

	snake->Body[0].pos_x += delta_x;
	snake->Body[0].pos_y += delta_y;
	
	for(int i=1; i<snake->size; i++)
	{
		snake->Body[i].pos_x = snake->Body[i-1].pos_x - delta_x;
		snake->Body[i].pos_y = snake->Body[i-1].pos_y - delta_y;
		snake->Body[i].body = snake->Body[i-1].body;
	}

}

bool isGameOver(Snake* snake)
{
	// eat itself
	for(int i=1; i<snake->size; i++)
	{
		if( snake->Body[0].pos_x == snake->Body[i].pos_x && 
			snake->Body[0].pos_y == snake->Body[i].pos_y)
			return TRUE;
	}

	// out of table
	if(snake->Body[0].pos_x < 0 ||  snake->Body[0].pos_y < 0)
		return TRUE;

	// out of table
	if(snake->Body[0].pos_x >= WIDTH)
		return TRUE;

	// out of table
	if(snake->Body[0].pos_y >= HEIGHT)
		return TRUE;

	return FALSE;
}

// print table
void print_table(WINDOW* main_win, char** table, Snake* snake)
{
	box(main_win, 0, 0);

	insert_snake(table, snake);

	// print table
	for(int y=0; y<HEIGHT; y++)
		for(int x=0; x<WIDTH; x++)
		{
			mvwprintw(main_win, y, x, "%c", table[y][x]);
		}

	wrefresh(main_win);
}

void insert_snake(char** table, Snake* snake)
{
	for(int i=0; i<HEIGHT; i++)
		for(int j=0; j<WIDTH; j++)
			table[i][j] = '.';

	// put snake on the table
	for(int i=0; i<snake->size; i++)
	{
		table[snake->Body[i].pos_y][snake->Body[i].pos_x] = snake->Body[i].body;
	}
}

char** create_table(int height, int width, char aux)
{
	char** table = (char**) malloc(height * sizeof(char*));

	for(int y=0; y<height; y++)
	{
		table[y] = (char*) malloc((width) * sizeof(char));

		for(int x=0; x<width; x++)
		{
			table[y][x] = aux;
		}
	}

	return table;
}

Snake create_snake(int height, int width)
{
	Snake snake;

	snake.size = 0;
	snake.direction = 'R';

	snake.Body = (Snake_Body*) malloc(3*sizeof(Snake_Body));

	for(int i=0; i<3; i++)
	{
		snake.Body[i].pos_x = (width/2) - i;
		snake.Body[i].pos_y = (height/2);

		if(i==0)
			snake.Body[i].body = '>';

		else
			snake.Body[i].body = '>';

		snake.size++;
	}

	return snake;
}