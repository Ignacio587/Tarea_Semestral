#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/select.h>  //biblioteca para movimiento continuo.
//#include <conio.h>
/*
PARA LINUX*/
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define YELLOW "\x1B[33m"
#define BLUE   "\x1b[34m"
#define MAGENTA "\x1B[35m"
#define RED     "\x1B[31m"
#define CYAN "\x1B[36m" 
#define NORMAL "\e[0m"
#define ORANGE  "\x1B[38;2;255;128;0m"
#define RESET "\033[0m"

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
#define muerto -5


#define BLINKY 51	// Rojo
#define PINKY 52	// Rosado
#define INKY 53		// Celeste
#define CLYDE 54	// Naranja
int vidas=2;

//ACTIVAR EN LINUX

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


// Crear matriz dinámica de nFilas por nColumnas
int** CrearMatriz(int nFilas, int nColumnas)
{
    int** matriz = (int**)malloc(sizeof(int*)*nFilas);
    for(int i = 0; i < nFilas; i++)
    {
        matriz[i] = (int*)malloc(sizeof(int)*nColumnas);
    }
    return matriz;
}

// Leer mapa desde archivo matrizTxt[] y recibirlo en matrizActual
void RecibirMapa(char matrizTxt[], int** matrizActual, int nFilas, int nColumnas)
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

// Leer posiciones de pacman y fantasmas desde archivo posicionesTxt
void RecibirPosiciones(char posicionesTxt[], int** matrizActual)
{
    FILE* archivo = fopen(posicionesTxt, "r");
    if(archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", posicionesTxt);
        exit(1);
    }

    for(int i=0; i<5; i++)
    {
        int x, y;
        fscanf(archivo, "%d", &x);
        fscanf(archivo, "%d", &y);

        switch(i) {
        case 0: matrizActual[x][y] = PACMAN; break;
        case 1: matrizActual[x][y] = BLINKY; break;
        case 2: matrizActual[x][y] = PINKY; break;
        case 3: matrizActual[x][y] = INKY; break;
        case 4: matrizActual[x][y] = CLYDE; break;
        default: matrizActual[x][y] = VACIO; break;
        }
    }
    fclose(archivo);
}

// Sobreescribir mapa actual para que quede guardado
void ActualizarMapa(char matrizTxt[], int** matrizActual, int nFilas, int nColumnas)
{
    FILE* archivo = fopen(matrizTxt, "w");
    if(archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", matrizTxt);
        exit(1);
    }

    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            fprintf(archivo, "%d ", matrizActual[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}


/*
// Sobreescribir posiciones actuales para que queden guardadas
void ActualizarPosiciones(char posicionesTxt[], int** matrizActual)
{
    FILE* archivo = fopen(posicionesTxt, "w");
    if(archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", posicionesTxt);
        exit(1);
    }

    for(int i=0; i<5; i++)
    {
        switch(i) {
        case 0:  break;
        case 1:  break;
        case 2:  break;
        case 3:  break;
        case 4:  break;
        default:  break;
        }

        fprintf(archivo, "%d %d", x, y);
    }
    fclose(archivo);
}
*/

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
				    printf(BLUE"██"RESET);	//	██
				    break;
				case VACIO:
				    printf("  ");
				    break;
				case PUNTO:
					printf(". ");	//	•
					break;
				case PUNTO_GRANDE:
					printf("⬤ ");	//	⬤
					break;
				case BARRERA:
					printf("--");	//	░░
					break;
				case PACMAN:
					printf(YELLOW"+ "RESET);	//	ᗧ
					break;
				case BLINKY:
					printf(RED"& "RESET);	//	ᗣ
					break;
				case PINKY:
					printf(MAGENTA"& "RESET);	//	ᗣ
					break;
				case INKY:
					printf(CYAN"& "RESET);	//	ᗣ
					break;
				case CLYDE:
					printf(ORANGE"& "RESET);	//	ᗣ
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
void BuscarPacman(int** matriz, int nFilas, int nColumnas, int* x, int* y)
{
    for(int i = 0; i < nFilas; i++)
    {
        for(int j = 0; j < nColumnas; j++)
        {
            if(matriz[i][j] == PACMAN)
            {
                *x = i;
                *y = j;
                
                return;
            }else{
                *x=muerto;
                *y=muerto;
            }
        }
    }
}
void LimpiarPacmanFantasmas(int** matriz, int nFilas,int nColumnas)
{
  for(int i=0; i<nFilas; i++)
  {
    for(int j=0; i<nColumnas; i++)
    {
     if(matriz[i][j]==PACMAN || matriz[i][j]==INKY || matriz[i][j]==PINKY || matriz[i][j]==BLINKY || matriz[i][j]==CLYDE)
      {
        matriz[i][j]==VACIO;
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
        if(matriz[newX][newY] == PARED)
        {   
             MoverFantasma(matriz, nFilas, nColumnas, FANTASMA);
        }else if(matriz[newX][newY]==PUNTO){
            matriz[xFantasma][yFantasma] = PUNTO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA;     // Mover Fantasma
        }else if(matriz[newX][newY] == VACIO){
            matriz[xFantasma][yFantasma] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA;  
        }else if(matriz[newX][newY]==BARRERA){
            matriz[xFantasma][yFantasma] = VACIO; // Dejar vacío la posición anterior
            matriz[newX][newY] = FANTASMA; 
        }else if (matriz[newX][newY] == BLINKY || matriz[newX][newY] == INKY || matriz[newX][newY] == PINKY || matriz[newX][newY] == CLYDE){
            MoverFantasma(matriz,nFilas,nColumnas,FANTASMA);
        }else if( matriz[newX][newY] == PUNTO_GRANDE){
            matriz[xFantasma][yFantasma] = PUNTO_GRANDE; 
            matriz[newX][newY] = FANTASMA; 
        }
         else if (matriz[newX][newY]== PACMAN){
            matriz[xFantasma][yFantasma]= VACIO;
            matriz[newX][newY] = FANTASMA;
            
         }   
    }
}


int main() {
   // Tamaño de la matriz
    int nFilas = 31, nColumnas = 28;

    // Crear matriz e inicializar con mapa inicial
    int** matrizPrincipal = CrearMatriz(nFilas, nColumnas);
    RecibirMapa("mapa1.txt", matrizPrincipal, nFilas, nColumnas);

    // Posición inicial Pacman y fantasmas
    RecibirPosiciones("posiciones.txt", matrizPrincipal);

    /* SOLO LINUX
    // Configurar terminal en modo raw para recibir input directo*/
    enableRawMode();
    

    // Puntaje y tiempo inicial
    int puntaje = 0, tiempo = 0;

    char input='d';
    struct timeval timeout;
    fd_set readfds;

    // Configura el tiempo de espera en milisegundos (por ejemplo, 500 ms)
    int timeout_ms = 500;
    // Ciclo principal del juego
    int running = true;
    while (vidas>0)
    {
        // Actualizar mapa_actual.txt con información nueva y insertarlo en matrizPrincipal.
        ActualizarMapa("mapa_actual.txt", matrizPrincipal, nFilas, nColumnas);
        RecibirMapa("mapa_actual.txt", matrizPrincipal, nFilas, nColumnas);

        system("clear"); // Limpiar pantalla LINUX
         // system("cls"); // Limpiar pantalla WINDOWS
        ImprimirInfo(puntaje, tiempo);
        ImprimirMatriz(matrizPrincipal, nFilas, nColumnas);
        printf("\n-- Utilice WASD para moverse y Q para cerrar el juego. Vidas: %d --\n", vidas);
        // Desde aquí vor a estudiar la funcion, y la struct timeval
         // Configurar el tiempo de espera (5 segundos en este ejemplo)
        timeout.tv_sec = timeout_ms / 1000;               // Segundos
        timeout.tv_usec = (timeout_ms % 1000) * 1000;    // Microsegundos

        // Reiniciar los file descriptors
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        
        fflush(stdout);
        // Usar select para esperar entrada
        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (result == -1) {
            perror("Error en select");
            exit(EXIT_FAILURE);
        } else if (result == 0) {
        } else {
            // Leer el carácter si hay entrada
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                input = getchar();
                if (input == 'q') {
                    printf("Saliendo del programa...\n");
                    break;
                }
            }
        }

        // Capturar tecla LINUX
        // char input = _getch(); // Capturar tecla WINDOWS
        //hasta aquí termina la implementacíon de la funcion 
        int direccion = 0;
        

        switch (input) {
            case 'w': direccion = UP; break;
            case 'a': direccion = LEFT; break;
            case 's': direccion = DOWN; break;
            case 'd': direccion = RIGHT; break;
            case 'q': running = false; break; // Salir del juego
        }

        if (direccion) {
            MoverJugador(matrizPrincipal, nFilas, nColumnas, direccion, &puntaje);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, BLINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, PINKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, INKY);
            MoverFantasma(matrizPrincipal, nFilas, nColumnas, CLYDE);
            tiempo++;
        }
        int xPacman, yPacman;
        BuscarPacman(matrizPrincipal, nFilas, nColumnas, &xPacman, &yPacman);
        if(xPacman == muerto && yPacman == muerto){

            vidas--;
            LimpiarPacmanFantasmas(matrizPrincipal,nFilas,nColumnas);
            RecibirPosiciones("posiciones.txt", matrizPrincipal);
            if(vidas==0){
                system("clear");
                printf("Game over\n");
            }
        }
    }
    usleep(500000); 

    /* SOLO LINUX
    // Restaurar terminal al modo normal*/
    disableRawMode();
    

    // Liberar memoria
    for(int i = 0; i < nFilas; i++)
        free(matrizPrincipal[i]);
    free(matrizPrincipal);


    return 0;
}

