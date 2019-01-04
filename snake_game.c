#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // using sleep()

typedef struct _snake_piece
{
	int pos_x;
	int pos_y;
	char piece; // '-', '|', '>','<','^','V'

} Snake_Piece;

typedef struct _snake
{
	int horiz; // +1 - moving right      -1 -moving left
	int verti; // +1 - moving up      -1 -moving down
	int size;

	Snake_Piece* Pieces;
	// struct with all snake parts and related chars

} Snake;

char** generate(int, int);
void print_table(char**, Snake*,int, int);
int eat_itself(Snake*);
int playing(char**,Snake*, int, int);
void move_snake(Snake*);
void add_piece(Snake*, int, int);

int main()
{
	int altura=30, largura=30;
	// puts("Digite a altura e largura do tabuleiro: ");
	// scanf("%d %d", &altura, &largura);

	char** tabuleiro = generate(altura, largura);

	Snake snake;
	snake.size = 0;
	snake.horiz = 1; // moving to the right
	snake.verti = 0; // not movint vertical

	// head and tail
	add_piece(&snake, altura, largura);

	while( playing(tabuleiro, &snake, altura, largura) )
	{
		move_snake(&snake);

		print_table(tabuleiro, &snake, altura, largura);
	}

	printf("GAME OVER\n");

	return 0;
}

void add_piece(Snake* snake, int altura, int largura)
{

	if(snake->size == 0)
	{
		// head and tail
		snake->Pieces=(Snake_Piece*)malloc(2*sizeof(Snake_Piece));

		
		// head
		snake->Pieces[0].pos_y = largura/2;

		snake->Pieces[0].pos_x = altura/2;
		snake->Pieces[0].piece = '>';

		snake->Pieces[1].pos_y = (largura/2) - 1;
		snake->Pieces[1].pos_x = altura/2;
		snake->Pieces[1].piece = '-';

		snake->size = 2;
	}

	else
	{	
		// adding new piece to Pieces array
		snake->Pieces=realloc(snake->Pieces, snake->size+1);
		snake->Pieces[snake->size+1] = snake->Pieces[snake->size];
		snake->size++;
	}

}

int playing(char** tabuleiro,Snake* snake, int altura, int largura)
{
	if(eat_itself(snake))
		return 0;

	// checking if snake head is on the board
	if( snake->Pieces[0].pos_x < 0 		  || 
		snake->Pieces[0].pos_y < 0 		  ||
		snake->Pieces[0].pos_x >= largura || 
		snake->Pieces[0].pos_y >= altura )
			return 0;

	return 1;
}

int eat_itself(Snake* snake)
{
	for(int i=1; i<snake->size; i++)
	{
		if( snake->Pieces[0].pos_x == snake->Pieces[i].pos_x && 
			snake->Pieces[0].pos_y == snake->Pieces[i].pos_y
		  )
		{
			return 1;
		}
	}

	return 0;
}

// moving all the snake pieces coordinates
void move_snake(Snake* snake)
{
	for(int i=0; i<snake->size; i++)
	{
		// x should be horiz -> fix later
		snake->Pieces[i].pos_x += snake->verti;
		snake->Pieces[i].pos_y += snake->horiz;
	}
}

void print_table(char** tabuleiro, Snake* snake,int altura, int largura)
{
	// clean the screen
	for(int i=0; i<100; i++) puts("\n");


	// reset the table
	for(int i=0; i<altura; i++)
		for(int j=0; j<largura; j++)
			tabuleiro[i][j] = '.';

	// add the snake pieces on the table
	for(int i=0; i<snake->size; i++)
	{
		printf("i: %d Pos_x: %d\n", i, snake->Pieces[i].pos_x);
		printf("i: %d Pos_y: %d\n", i, snake->Pieces[i].pos_y);

		tabuleiro[snake->Pieces[i].pos_x][snake->Pieces[i].pos_y] = snake->Pieces[i].piece;
	}

	// print the table
	for(int i=0; i<altura; i++)
	{
		for(int j=0; j<largura; j++)
		{
			printf("%c ", tabuleiro[i][j]);
		}

		printf("\n");
	}

	// wait 1 second
	sleep(1);
}

char** generate(int altura, int largura)
{
	char** tmp;

	tmp = malloc(altura * sizeof(char*)); // y

	for(int i=0; i<altura; i++)
	{
		tmp[i] = malloc((largura+1)*sizeof(char)); // x
		
		for(int j=0; j<largura; j++)
		{
			tmp[i][j] = '.';
		}
	}

	return tmp;
}