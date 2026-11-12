#include "../include/visuales.h"
#include "../include/logica.h"

static SDL_Window *ventana = NULL;
static SDL_Renderer *renderizador = NULL;
static SDL_Texture *fondo1 = NULL;    
static SDL_Texture *girasol_textura0 = NULL;
static SDL_Texture *lanzaguisantes_textura0 = NULL;
static SDL_Texture *nuez_textura0 = NULL;
static SDL_Texture *nuez_textura1 = NULL;
static SDL_Texture *nuez_textura2 = NULL;
static SDL_Texture *zombi_textura0 = NULL;
static SDL_Texture *guisante_textura0 = NULL;

static float offset_zombies[FILAS_JUEGO][COLUMNAS_JUEGO] = {0};

bool inicializarVisuales(void) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software"); 

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Error al iniciar SDL: %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer(NOMBRE_VENTANA, (int)ANCHO, (int)ALTO, 0, &ventana, &renderizador)){
        SDL_Log("Error en ventana: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);

    //Carga de texturas:

    fondo1 = cargarTextura("assets/fondo1.png");
    if(!fondo1){
        return false;
    }
    lanzaguisantes_textura0 = cargarTextura("assets/lanzaguisantes_textura0.png");
    if(!lanzaguisantes_textura0){
        return false;
    }
    girasol_textura0 = cargarTextura("assets/girasol_textura0.png");
    if(!girasol_textura0){
        return false;
    }
    nuez_textura0 = cargarTextura("assets/nuez_textura0.png");
    if(!nuez_textura0){
        return false;
    }
    nuez_textura1 = cargarTextura("assets/nuez_textura1.png");
    if(!nuez_textura1){
        return false;
    }
    nuez_textura2 = cargarTextura("assets/nuez_textura2.png");
    if(!nuez_textura2){
        return false;
    }
    zombi_textura0 = cargarTextura("assets/zombi_textura0.png");
    if(!zombi_textura0){
        return false;
    }
    guisante_textura0 = cargarTextura("assets/guisante_textura0.png");
    if(!guisante_textura0){
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

void cerrarSDL(void) {
    if(fondo1) {
        SDL_DestroyTexture(fondo1); 
        fondo1 = NULL;
    }
    if(renderizador) SDL_DestroyRenderer(renderizador);
    if(ventana) SDL_DestroyWindow(ventana);
    MIX_Quit();
    SDL_Quit();
}

void loopVisuales(float pos_camara_x){
    prepararFondo();

    if(fondo1) {
        RenderizarFondo(renderizador, fondo1, pos_camara_x);
    }

    for (int fila = 0;fila < FILAS_JUEGO;fila++) {
        for (int columna = 0; columna < COLUMNAS_JUEGO;columna++){    

            SDL_FRect rectangulo;
            rectangulo.x = (float)(MARGEN_X + columna * ANCHO_CELDA);
            rectangulo.y = (float)(MARGEN_Y + fila * ALTO_CELDA);
            rectangulo.w = (float)ANCHO_CELDA;
            rectangulo.h = (float)ALTO_CELDA;

            int elemento = obtener_celda(fila, columna);
            imprimirTextura(elemento, rectangulo);
        }
    }

    presentarPantalla();
}

bool animacionInicio(float *pos_camara_x){
    if(*pos_camara_x > POS_CAMARA_JUGANDO){
            *pos_camara_x -= VEL_CAMARA;
        } else if(*pos_camara_x <= POS_CAMARA_JUGANDO){
            *pos_camara_x = POS_CAMARA_JUGANDO;
            return false;
        }
    return true;
}

void imprimirTextura(int planta, SDL_FRect rectangulo){
    switch (planta) {
        case GIRASOL:
            SDL_RenderTexture(renderizador, girasol_textura0, NULL, &rectangulo);
            break;
        case LANZAGUISANTES:
            SDL_RenderTexture(renderizador, lanzaguisantes_textura0, NULL, &rectangulo);
            break;
        case NUEZ:
            SDL_RenderTexture(renderizador, nuez_textura0, NULL, &rectangulo);
            break;
        case ZOMBIE:
        case FLAG_ZOMBIE:
            SDL_RenderTexture(renderizador, zombi_textura0, NULL, &rectangulo);
            break;
        case GUISANTE:
            SDL_RenderTexture(renderizador, guisante_textura0, NULL, &rectangulo);
            break;
    }
}

void iniciarAnimacionZombie(int fila, int columna) {
    offset_zombies[fila][columna] = (float)ANCHO_CELDA; 
}