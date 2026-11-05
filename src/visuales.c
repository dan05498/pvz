#include "../include/visuales.h"

static SDL_Window *ventana = NULL;
static SDL_Renderer *renderizador = NULL;

bool inicializarVisuales(void) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software"); 

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error al iniciar SDL: %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer("PVZ", COLUMNAS * TAM_CASILLA, FILAS * TAM_CASILLA, 0, &ventana, &renderizador)){
        SDL_Log("Error en ventana: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);
    return true;
}

SDL_Texture *cargarTextura(const char *archivo) {
    SDL_Texture *textura = IMG_LoadTexture(renderizador, archivo);
    if(textura == NULL) {
        SDL_Log("Error al cargar %s: %s", archivo, SDL_GetError());
    }
    return textura;
}

void prepararFondo(void) {
    SDL_SetRenderDrawColor(renderizador, 30, 35, 40, 255);
    SDL_RenderClear(renderizador);
}

void dibujarCasilla(SDL_Texture *textura, int fila, int columna) {
    SDL_FRect rectangulo = {
        (float)(columna * TAM_CASILLA), 
        (float)(fila * TAM_CASILLA), 
        (float)TAM_CASILLA, 
        (float)TAM_CASILLA
    }; 
    SDL_RenderTexture(renderizador, textura, NULL, &rectangulo);
}

void presentarPantalla(void) {
    SDL_RenderPresent(renderizador);
    SDL_Delay(16);
}

void cerrarVisuales(void) {
    if(renderizador) SDL_DestroyRenderer(renderizador);
    if(ventana) SDL_DestroyWindow(ventana);
    SDL_Quit();
}