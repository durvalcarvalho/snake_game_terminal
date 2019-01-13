#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// MACROS
#define WIDTH 50
#define HEIGHT 20

char foodIcon = 'O';
int points = 0;

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

typedef struct _food_position
{
	int pos_x;
	int pos_y;
	
} Food_Position;


// FUNCTIONS
void initscreen(void)
{
	initscr();   // init ncurses mode
	clear();	 // clear the screen
	noecho();	 // disable type echo
	curs_set(0); // disable blink cursor
}

Food_Position generate_food(char** table, char foodIcon)
{
	srand(time(NULL));
	int randWidth, randHeight;

	// Looking for an empty random table spot to put a food
	do
	{
		// (1-49)
		randWidth = rand() % (WIDTH-1) + 1;
		randHeight = rand() % (HEIGHT-1) + 1;

	} while(table[randHeight][randWidth] != '.');

	Food_Position food = { randWidth, randHeight };

	table[randHeight][randWidth] = foodIcon;

	return food;
}

bool isGameOver(Snake* snake)
{
	// eat itself
	for(int i=1; i<snake->size; i++)
	{
		if(snake->Body[0].pos_x == snake->Body[i].pos_x && 
		snake->Body[0].pos_y == snake->Body[i].pos_y)
			return TRUE;
	}

	// out of table
	if(snake->Body[0].pos_x==0||snake->Body[0].pos_y==0)
		return TRUE;

	// out of table
	if(snake->Body[0].pos_x >= WIDTH-1)
		return TRUE;

	// out of table
	if(snake->Body[0].pos_y >= HEIGHT-1)
		return TRUE;

	return FALSE;
}

void increase_snake(Snake* snake)
{
	int Ssize = snake->size + 1;
	snake->size++;

	mvprintw(4, 4, "%d", Ssize);
	refresh();

	// problem with reallocation
	// snake->Body=(Snake_Body*)realloc(snake->Body,  Ssize*sizeof(Snake_Body));

	snake->Body[Ssize].pos_x = snake->Body[Ssize-1].pos_x;
	snake->Body[Ssize].pos_y = snake->Body[Ssize-1].pos_y;
	snake->Body[Ssize].body  = snake->Body[Ssize-1].body;	
}

void movement_snake(Snake* snake, char** table)
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

	// Taking the snake off the table
	for(int i=0; i<snake->size; i++)
	{
		table[snake->Body[i].pos_y][snake->Body[i].pos_x] = '.';
	}

	// Moving the body
	for(int i=(snake->size)-1; i>0; i--)
	{
		snake->Body[i].pos_x = snake->Body[i-1].pos_x;
		snake->Body[i].pos_y = snake->Body[i-1].pos_y;

		snake->Body[i].body = snake->Body[i-1].body;
	}

	// eating the food
	if(table[snake->Body[0].pos_y + delta_y][snake->Body[0].pos_x + delta_x] == foodIcon)
	{
		points++;
		increase_snake(snake);
	}

	// Moving the head
	snake->Body[0].pos_x += delta_x;
	snake->Body[0].pos_y += delta_y;
}

void insert_snake(char** table, Snake* snake)
{
	// Putting the snake on the table
	for(int i=0; i<snake->size; i++)
	{
		table[snake->Body[i].pos_y][snake->Body[i].pos_x] = snake->Body[i].body;
	}
}

void print_table(WINDOW* main_win, char** table, Snake* snake)
{
	insert_snake(table, snake);

	// print table
	for(int y=0; y<HEIGHT; y++)
		for(int x=0; x<WIDTH; x++)
		{
			mvwprintw(main_win, y, x, "%c", table[y][x]);
		}

	wrefresh(main_win);
}

char** create_table(int height, int width, char aux)
{
	char** table = (char**) malloc(height * sizeof(char*));

	for(int y=0; y<height; y++)
	{
		table[y] = (char*) malloc((width) * sizeof(char));

		for(int x=0; x<width; x++)
		{
			// corners
			if((x==0 && y+1==height)||(x+1==width && y==0)||(x+1==width && y+1==height)||(x==0 && y==0))
				table[y][x] = '+';

			// horizontal borders
			else if( (x==0) || (x+1==width))
				table[y][x] = '|';

			// vertical borders
			else if( (y==0) || (y+1==height))
				table[y][x] = '-';
			
			else
				table[y][x] = aux;
		}
	}

	return table;
}

Snake create_snake(int height, int width)
{
	Snake snake;
	snake.size = 0;
	snake.direction = 'R'; // starts moving right

	int initSize = 5;

	snake.Body = (Snake_Body*) malloc(50*sizeof(Snake_Body));

	for(int i=0; i<initSize; i++)
	{
		// placing the initial positions
		snake.Body[i].pos_x = (width/2) - i;
		snake.Body[i].pos_y = (height/2);
		snake.Body[i].body = '>';
		snake.size++;
	}

	return snake;
}