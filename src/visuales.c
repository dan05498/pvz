#include "../include/visuales.h"

static SDL_Window *ventana = NULL;
static SDL_Renderer *renderizador = NULL;
static SDL_Texture *fondo1 = NULL;

bool inicializarVisuales(void) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software"); 

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error al iniciar SDL: %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer("PVZ", (int)ANCHO, (int)ALTO, 0, &ventana, &renderizador)){
        SDL_Log("Error en ventana: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);

    fondo1 = cargarTextura("assets/fondo1.png");
    if(!fondo1){
        return false;
    }

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

void RenderizarFondo(SDL_Renderer* renderizador, SDL_Texture* fondo1, float pos_camara_x) {
    SDL_FRect rect_origen = {
        pos_camara_x, 
        0.0f,
        ANCHO,
        ALTO
    };

    SDL_FRect rect_destino = {
        0.0f,
        0.0f,
        ANCHO,
        ALTO
    };

    SDL_RenderTexture(renderizador, fondo1, &rect_origen, &rect_destino);
}

void presentarPantalla(void) {
    SDL_RenderPresent(renderizador);
    SDL_Delay(16);
}

void RenderizarCuadriculaDebug(SDL_Renderer* renderizador) {
    SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255); 

    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            
            SDL_FRect rect_celda = {
                MARGEN_X + (col * ANCHO_CELDA),
                MARGEN_Y + (fila * ALTO_CELDA),
                ANCHO,
                ALTO
            };
            
            SDL_RenderRect(renderizador, &rect_celda);
        }
    }
}

void cerrarVisuales(void) {
    if(fondo1) {
        SDL_DestroyTexture(fondo1); 
        fondo1 = NULL;
    }
    if(renderizador) SDL_DestroyRenderer(renderizador);
    if(ventana) SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void loopVisuales(float pos_camara_x){
    prepararFondo();

    if(fondo1) {
        RenderizarFondo(renderizador, fondo1, pos_camara_x);
        
        if (pos_camara_x == 210.0f) {
            RenderizarCuadriculaDebug(renderizador);
        }
    }

    presentarPantalla();
}

bool animacionInicio(float *pos_camara_x, float pos_camara_jugando, float vel_camara){
    if(*pos_camara_x > pos_camara_jugando){
            *pos_camara_x -= vel_camara;
        } else if(*pos_camara_x <= pos_camara_jugando){
            *pos_camara_x = pos_camara_jugando;
            return false;
        }
    return true;
}