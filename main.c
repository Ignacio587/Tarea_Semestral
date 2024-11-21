#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

// Configurar terminal en modo raw (capturar teclas sin Enter)
void enableRawMode() {
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    struct termios raw = termiosOriginal;
    raw.c_lflag &= ~(ICANON | ECHO);           // Desactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Aplicar cambios
}

// Restaurar terminal al modo normal
void disableRawMode() {
    struct termios termiosOriginal;
    tcgetattr(STDIN_FILENO, &termiosOriginal); // Obtener configuración actual
    termiosOriginal.c_lflag |= (ICANON | ECHO); // Reactivar modo canónico y eco
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal); // Aplicar cambios
}

// Crear matriz dinámica
int** CrearMatriz(int nFilas, int nColumnas) {
    int** matriz = (int**)malloc(sizeof(int*) * nFilas);
    for (int i = 0; i < nFilas; i++) {
        matriz[i] = (int*)malloc(sizeof(int) * nColumnas);
    }
    return matriz;
}

// Leer matriz desde archivo
void RecibirMatriz(int** matrizActual, int nFilas, int nColumnas) {
    FILE* archivo = fopen("matriz_inicial.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo matriz_inicial.txt\n");
        exit(1);
    }

    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {
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
				    printf("██");	//	█
				    break;
				case VACIO:
				    printf("  ");
				    break;
				case PUNTO:
					printf("• ");	//	•
					break;
				case PUNTO_GRANDE:
					printf("⬤ ");	//	⬤
					break;
				case BARRERA:
					printf("░");	//	░
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

// Buscar la posición de Pac-Man
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y) {
    for (int i = 0; i < nFilas; i++) {
        for (int j = 0; j < nColumnas; j++) {
            if (matriz[i][j] == PACMAN) {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// Mover a Pac-Man en la matriz
void MoverJugador(int** matriz, int nFilas, int nColumnas, int direccion) {
    
    int xPacman, yPacman;
    BuscarPacman(matriz, nFilas, nColumnas, &xPacman, &yPacman);

    // Determinar nueva posición
    int newX = xPacman, newY = yPacman;
    if (direccion == UP) newX--;
    else if (direccion == DOWN) newX++;
    else if (direccion == LEFT) newY--;
    else if (direccion == RIGHT) newY++;

    // Validar movimiento
    if (newX >= 0 && newX < nFilas && newY >= 0 && newY < nColumnas) {
        if (matriz[newX][newY] == VACIO || matriz[newX][newY] == PUNTO) {
            matriz[xPacman][yPacman] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = PACMAN;     // Mover Pac-Man
        } else if (matriz[newX][newY] == PARED) {
        }
    }
}

int main() {
    // Tamaño de la matriz
    int nFilas = 31, nColumnas = 28;

    // Crear matriz e inicializar
    int** matrizPrincipal = CrearMatriz(nFilas, nColumnas);
    RecibirMatriz(matrizPrincipal, nFilas, nColumnas);

    // Posición inicial de Pac-Man
    matrizPrincipal[23][13] = PACMAN;

    // Configurar terminal en modo raw
    enableRawMode();


	matrizPrincipal[11][13] = BLINKY;
	matrizPrincipal[14][11] = INKY;
	matrizPrincipal[14][13] = PINKY;
	matrizPrincipal[14][15] = CLYDE;

    // Ciclo principal del juego
    int running = 1;
    while (running) {
        system("clear"); // Limpiar pantalla
        ImprimirMatriz(matrizPrincipal, nFilas, nColumnas);

        char key = getchar(); // Capturar tecla
        int direccion = 0;

        switch (key) {
            case 'w': direccion = UP; break;
            case 'a': direccion = LEFT; break;
            case 's': direccion = DOWN; break;
            case 'd': direccion = RIGHT; break;
            case 'q': running = 0; break; // Salir del juego
            default: printf("Tecla no válida\n");
        }

        if (direccion) {
            MoverJugador(matrizPrincipal, nFilas, nColumnas, direccion);
        }
    }

    // Restaurar terminal al modo normal
    disableRawMode();

    // Liberar memoria
    for (int i = 0; i < nFilas; i++) free(matrizPrincipal[i]);
    free(matrizPrincipal);

    return 0;
}
