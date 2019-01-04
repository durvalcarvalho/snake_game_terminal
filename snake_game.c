#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _snake
{
	int horiz;
	int verti;

	int size;
	int pos_x;
	int pos_y;

} Snake;

char** generate(int, int);

void print_table(char**, int, int);

void snake_pos(Snake*);

int playing(char**, Snake*);

int altura, largura;

int main()
{
	puts("Digite a altura e largura do tabuleiro: ");
	
	scanf("%d %d", &altura, &largura);

	char** tabuleiro = generate(altura, largura);

	Snake snake = {0, -1, 1, altura/2, largura/2 };

	while( playing(tabuleiro, &snake) )
	{
		print_table(tabuleiro, altura, largura);
	}

	return 0;
}

void snake_pos(Snake* snake)
{
	if( snake->pos_x + snake->horiz >=0 &&
	    snake->pos_x + snake->horiz <= largura-1 )
	{
		snake->pos_x += snake->horiz;
	}

	if(	snake->pos_y + snake->verti >=0 &&
		snake->pos_y + snake->verti <= altura-1 )
	{
		snake->pos_y += snake->verti;
	}
}

int playing(char** tabuleiro, Snake* snake)
{
	//               lin	  col
	if(snake->horiz == 1)
	{
		tabuleiro[snake->pos_y][snake->pos_x] = '-';
		snake_pos(snake);
		tabuleiro[snake->pos_y][snake->pos_x] = '>';
	}
	else if(snake->horiz == -1)
	{
		tabuleiro[snake->pos_y][snake->pos_x] = '-';
		snake_pos(snake);
		tabuleiro[snake->pos_y][snake->pos_x] = '<';
	}
	else if(snake->verti == 1)
	{
		tabuleiro[snake->pos_y][snake->pos_x] = '|';
		snake_pos(snake);
		tabuleiro[snake->pos_y][snake->pos_x] = 'V';
	}
	else if(snake->verti == -1)
	{
		tabuleiro[snake->pos_y][snake->pos_x] = '|';
		snake_pos(snake);
		tabuleiro[snake->pos_y][snake->pos_x] = '^';
	}
}

void print_table(char** tabuleiro, int altura, int largura)
{
	for(int i=0; i<100; i++) puts("\n");

	for(int i=0; i<altura; i++)
	{
		for(int j=0; j<largura; j++)
		{
			printf("%c ", tabuleiro[i][j]);
		}

		printf("\n");
	}

	sleep(1);

}

char** generate(int altura, int largura)
{
	char** tmp;

	tmp = malloc(altura * sizeof(char*));

	for(int i=0; i<altura; i++)
	{
		tmp[i] = malloc((largura+1)*sizeof(char));
		
		for(int j=0; j<largura; j++)
		{
			tmp[i][j] = '.';
		}
	}

	return tmp;
}