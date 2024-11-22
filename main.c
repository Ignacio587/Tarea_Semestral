#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* PARA WINDOWS
*/
#include <ctype.h>
#include <conio.h>
/*
PARA LINUX
#include <termios.h> 
#include <unistd.h>
*/

// Constantes
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

/*
ACTIVAR EN LINUX

// Configurar terminal en modo raw (capturar teclas sin Enter)
void enableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    struct termios raw = termiosOriginal;
    raw.c_lflag &= ~(ICANON | ECHO);           // Desactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Aplicar cambios
}

// Restaurar terminal al modo normal
void disableRawMode()
{
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    termiosOriginal.c_lflag |= (ICANON | ECHO); // Reactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal); // Aplicar cambios
}
*/

// Crear matriz dinámica
int** CrearMatriz(int nFilas, int nColumnas)
{
    int** matriz = (int**)malloc(sizeof(int*)*nFilas);
    for(int i = 0; i < nFilas; i++)
    {
        matriz[i] = (int*)malloc(sizeof(int)*nColumnas);
    }
    return matriz;
}

// Leer matriz desde archivo
void RecibirMatriz(char matrizTxt[], int** matrizActual, int nFilas, int nColumnas)
{
    FILE* archivo = fopen(matrizTxt, "r");
    if(archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", matrizTxt);
        exit(1);
    }

    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            fscanf(archivo, "%d", &matrizActual[i][j]);
        }
    }
    fclose(archivo);
}

// Imprimir la matriz
void ImprimirMatriz(int** matriz, int nFilas, int nColumnas)
{
	for(int i=0; i<nFilas; i++)
	{
		for(int j=0; j<nColumnas; j++) 
		{
		 	switch(matriz[i][j])
			{
				case PARED:
				    printf("[]");	//	██
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
					printf("--");	//	░░
					break;
				case PACMAN:
					printf("+ ");	//	ᗧ
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

// Imprimir información
void ImprimirInfo(int puntaje, int tiempo)
{
    printf("\n");
    printf("====================  P A C M A N   ====================\n\n");    // Pᗣᗧ•••MᗣN
    printf("     Puntaje: %d                         Tiempo: %d \n\n", puntaje, tiempo);
}

// Buscar la posición de Pac-Man
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y) {
    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            if(matriz[i][j] == PACMAN)
            {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// Mover a Pac-Man en la matriz
void MoverJugador(int** matriz, int nFilas, int nColumnas, int direccion, int* puntaje)
{
    
    int xPacman, yPacman;
    BuscarPacman(matriz, nFilas, nColumnas, &xPacman, &yPacman);

    // Determinar nueva posición
    int newX = xPacman, newY = yPacman;
    if(direccion == UP){
        newX--;
        if(newX==-1) newX=nFilas-1;
    }
    else if(direccion == DOWN){
        newX++;
        if(newX==nFilas) newX=0;
    }
    else if(direccion == LEFT){
        newY--;
        if(newY==-1) newY=nColumnas-1;
    }
    else if(direccion == RIGHT){
        newY++;
        if(newY==nColumnas) newY=0;
    }

    // Validar movimiento
    if (newX >= 0 && newX < nFilas && newY >= 0 && newY < nColumnas)
    {
        if(matriz[newX][newY] == VACIO)
        {
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
        }
        else if(matriz[newX][newY] == PUNTO)
        {
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
            *puntaje+=10;       // Aumentar puntaje en 10
        }
        else if(matriz[newX][newY] == PUNTO_GRANDE)
        {
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
            *puntaje+=50;       // Aumentar puntaje en 50
        }
    }
}

// Buscar la posición de los fantasmas
void BuscarFantasma(int** matriz, int nFilas, int nColumnas, int* x, int* y, int FANTASMA) {
    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            if(matriz[i][j] == FANTASMA)
            {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// Mover a los Fantasmas en la matriz
void MoverFantasma(int** matriz, int nFilas, int nColumnas, int FANTASMA)
{

    int xFantasma, yFantasma;
    BuscarFantasma(matriz, nFilas, nColumnas, &xFantasma, &yFantasma, FANTASMA);

    int direccion = (rand()%4)+1;
    // Determinar nueva posición
    int newX = xFantasma, newY = yFantasma;
    if(direccion == UP){
        newX--;
        if(newX==-1) newX=nFilas-1;
    }
    else if(direccion == DOWN){
        newX++;
        if(newX==nFilas) newX=0;
    }
    else if(direccion == LEFT){
        newY--;
        if(newY==-1) newY=nColumnas-1;
    }
    else if(direccion == RIGHT){
        newY++;
        if(newY==nColumnas) newY=0;
    }

    if (newX >= 0 && newX < nFilas && newY >= 0 && newY < nColumnas)
    {
        if(matriz[newX][newY] != PARED)
        {
            matriz[xFantasma][yFantasma] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA;     // Mover Fantasma
        }
        else
            MoverFantasma(matriz, nFilas, nColumnas, FANTASMA);
    }
}


int main() {
    // Tamaño de la matriz
    int nFilas = 31, nColumnas = 28;

    // Crear matriz e inicializar
    int** matrizPrincipal = CrearMatriz(nFilas, nColumnas);
    RecibirMatriz("matriz_inicial.txt", matrizPrincipal, nFilas, nColumnas);

    // Posición inicial Pacman
    matrizPrincipal[23][13] = PACMAN;

    // Posición inicial fantasmas
	matrizPrincipal[11][13] = BLINKY;
	matrizPrincipal[14][11] = INKY;
	matrizPrincipal[14][13] = PINKY;
	matrizPrincipal[14][15] = CLYDE;

    /* SOLO LINUX
    // Configurar terminal en modo raw para recibir input directo
    enableRawMode();
    */

    // Puntaje y tiempo inicial
    int puntaje = 0, tiempo = 0;


    // Ciclo principal del juego
    int running = true;
    while (running)
    {
        // system("clear"); // Limpiar pantalla LINUX
        system("cls"); // Limpiar pantalla WINDOWS
        ImprimirInfo(puntaje, tiempo);
        ImprimirMatriz(matrizPrincipal, nFilas, nColumnas);
        printf("\n-- Utilice WASD para moverse y Q para cerrar el juego --\n");

        // char input = getchar(); // Capturar tecla LINUX
        char input = _getch(); // Capturar tecla WINDOWS
        int direccion = 0;

        switch (input) {
            case 'w': direccion = UP; break;
            case 'a': direccion = LEFT; break;
            case 's': direccion = DOWN; break;
            case 'd': direccion = RIGHT; break;
            case 'q': running = false; break; // Salir del juego
            default: printf("Tecla no válida\n");
        }

        if (direccion) {
            MoverJugador(matrizPrincipal, nFilas, nColumnas, direccion, &puntaje);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, BLINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, PINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, INKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, CLYDE);
            tiempo++;
        }
    }

    /* SOLO LINUX
    // Restaurar terminal al modo normal
    disableRawMode();
    */

    // Liberar memoria
    for(int i = 0; i < nFilas; i++)
        free(matrizPrincipal[i]);
    free(matrizPrincipal);


    return 0;
}
