//	Pᗣᗧ•••MᗣN

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

#define PARED -1
#define VACIO 0
#define PUNTO 1
#define PUNTO_GRANDE 11
#define PACMAN 2
#define BARRERA 3

#define BLINKY 51	// Rojo
#define PINKY 52	// Rosado
#define INKY 53		// Celeste
#define CLYDE 54	// Naranja


int** CrearMatriz(int nFilas, int nColumnas)
{
	int** matriz;
	matriz = (int**)malloc(sizeof(int*)*nFilas);
	for(int i=0; i<nFilas; i++)
	{
		matriz[i] = (int*)malloc(sizeof(int)*nColumnas);
	}
	return matriz;
}

void RecibirMatriz(int** matrizActual, int nX, int nY)
{
	FILE *matrizRecibida;
	matrizRecibida = fopen("matriz_inicial.txt", "r");


	int var = 0;
	if(matrizRecibida != NULL)
	{
		int cx = 0;
		int cy = 0;

		while(fscanf(matrizRecibida, "%d", &var) != EOF)
		{
			matrizActual[cx][cy] = var;

			if(cy != nY-1)
			{
				cy++;
			}
			else
			{
				cy=0;
				cx++;
			}
		}
	}
	fclose(matrizRecibida);	
}

void ImprimirMatriz(int** matriz, int nFilas, int nColumnas)
{
	for(int i=0; i<nFilas; i++)
	{
		for(int j=0; j<nColumnas; j++) 
		{
		 	switch(matriz[i][j])
			{
				case PARED:
				    printf("[]");	//	█
				    break;
				case VACIO:
				    printf("  ");
				    break;
				case PUNTO:
					printf(". ");	//	•
					break;
				case PUNTO_GRANDE:
					printf("o ");	//	⬤
					break;
				case BARRERA:
					printf("==");	//	░
					break;
				case PACMAN:
					printf("O ");	//	ᗧ
					break;
				case BLINKY:
					printf("& ");	//	ᗣ
					break;
				case PINKY:
					printf("& ");	//	ᗣ
					break;
				case INKY:
					printf("& ");	//	ᗣ
					break;
				case CLYDE:
					printf("& ");	//	ᗣ
					break;
				default:
					printf("%d ", matriz[i][j]);
			}
		}


	printf("\n");
	}
}

void RetornarMatriz() //	To do
{
	/*
	retornar matriz con file en un archivo llamado matriz_actual.txt
	el cual iré usando en RecibirMatriz, para seguir con el loop
	*/
}

int RecibirInputWASD()
{
	char input;
	printf("Move to? ");
	input = getchar();

	int aux;
	while((aux = getchar())!='\n' && aux!=EOF)
	{
		/*limpiar el buffer*/
	}

	if(input=='w')
	{
		return UP;	//UP
	}
	else if(input=='a')
	{
		return LEFT;	//LEFT
	}
	else if(input=='s')
	{
		return DOWN;	//DOWN
	}
	else if(input=='d')
	{
		return RIGHT;	//RIGHT
	}
	else
	{
		printf("Input invalido\n");
		return -1;
	}
}

void MoverJugador(int** matriz, int nX, int nY, int inputActual)
{
	int xPacman; 
	int yPacman;

	for(int i=0; i<nX; i++)
	{
		for(int j=0; j<nY; j++)
		{
			if(matriz[i][j]==PACMAN)
			{
				xPacman = i;
				yPacman = j;
				break;
			}
		}
	}

	/*
	Agregar validaciones si es o no posible mover a Pacman.

	Tenemos 3 casos:
		1. Si se puede.
			-> Si hay punto, lo reemplazamos por aire.
			-> Si no hay punto, no pasa nada y sólo avanzas.
	
		2. No se puede porque hay pared.
			-> Avanzan los fantasmas, pero tú no.

		3. No se puede porque hay fantasma.
			-> Pierdes una vida, o fin del juego.

	*/

	if(inputActual==UP)
	{
		if(matriz[xPacman-1][yPacman] == PARED)
		{
			matriz[xPacman][yPacman] = PACMAN;
		}
		else if(matriz[xPacman-1][yPacman] == VACIO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman-1][yPacman] = PACMAN;
		}
		else if(matriz[xPacman-1][yPacman] == PUNTO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman-1][yPacman] = PACMAN;
		}
	}
	else if(inputActual==LEFT)
	{
		if(matriz[xPacman][yPacman-1] == PARED)
		{
			matriz[xPacman][yPacman] = PACMAN;
		}
		else if(matriz[xPacman][yPacman-1] == VACIO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman][yPacman-1] = PACMAN;
		}
		else if(matriz[xPacman][yPacman-1] == PUNTO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman][yPacman-1] = PACMAN;
		}
	}
	else if(inputActual==DOWN)
	{
		if(matriz[xPacman+1][yPacman] == PARED)
		{
			matriz[xPacman][yPacman] = PACMAN;
		}
		else if(matriz[xPacman+1][yPacman] == VACIO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman+1][yPacman] = PACMAN;
		}
		else if(matriz[xPacman+1][yPacman] == PUNTO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman+1][yPacman] = PACMAN;
		}
	}
	else if(inputActual==RIGHT)
	{
		if(matriz[xPacman][yPacman+1] == PARED)
		{
			matriz[xPacman][yPacman] = PACMAN;
		}
		else if(matriz[xPacman][yPacman+1] == VACIO)
		{	
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman][yPacman+1] = PACMAN;
		}
		else if(matriz[xPacman][yPacman+1] == PUNTO)
		{
			matriz[xPacman][yPacman] = VACIO;
			matriz[xPacman][yPacman+1] = PACMAN;
		}
	}
}




int main()
{
	int nx = 31;
	int ny = 28;

	int** matrizPrincipal = CrearMatriz(nx, ny);
	RecibirMatriz(matrizPrincipal, nx, ny);


	matrizPrincipal[23][13] = PACMAN;

	matrizPrincipal[11][13] = BLINKY;
	matrizPrincipal[14][11] = INKY;
	matrizPrincipal[14][13] = PINKY;
	matrizPrincipal[14][15] = CLYDE;


	int input = 0;
	while(input!=(-1))
	{
		ImprimirMatriz(matrizPrincipal, nx, ny);
		input = RecibirInputWASD();
		printf("\n");
		MoverJugador(matrizPrincipal, nx, ny, input);
	}


	return 0;
}