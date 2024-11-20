#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define RECT_SIZE 50
#define SPEED 5 // Velocidad constante del rectángulo

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error inicializando SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Movimiento constante", 
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED, 
                                       WINDOW_WIDTH, 
                                       WINDOW_HEIGHT, 
                                       SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "Error creando ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "Error creando renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Rect rect = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, RECT_SIZE, RECT_SIZE}; // Rectángulo centrado

    int quit = 0;
    SDL_Event event;

    // Variables para la velocidad del rectángulo
    int velocityX = SPEED; // Velocidad inicial en X
    int velocityY = 0;     // Velocidad inicial en Y

    while (!quit) {
        // Manejo de eventos
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN) {
                // Cambiar dirección según la tecla presionada
                if (event.key.keysym.sym == SDLK_w) { // Arriba
                    velocityX = 0;
                    velocityY = -SPEED;
                } else if (event.key.keysym.sym == SDLK_s) { // Abajo
                    velocityX = 0;
                    velocityY = SPEED;
                } else if (event.key.keysym.sym == SDLK_a) { // Izquierda
                    velocityX = -SPEED;
                    velocityY = 0;
                } else if (event.key.keysym.sym == SDLK_d) { // Derecha
                    velocityX = SPEED;
                    velocityY = 0;
                }
            }
        }

        // Actualizar posición del rectángulo
        rect.x += velocityX;
        rect.y += velocityY;

        // Limitar el rectángulo dentro de los bordes de la ventana
        if (rect.x < 0) rect.x = 0;
        if (rect.y < 0) rect.y = 0;
        if (rect.x + rect.w > WINDOW_WIDTH) rect.x = WINDOW_WIDTH - rect.w;
        if (rect.y + rect.h > WINDOW_HEIGHT) rect.y = WINDOW_HEIGHT - rect.h;

        // Dibujar
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // Fondo blanco
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);     // Rectángulo rojo
        SDL_RenderFillRect(ren, &rect);
        SDL_RenderPresent(ren);

        // Control de velocidad del bucle
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    // Liberar recursos
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
