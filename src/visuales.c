#include "../include/visuales.h"
#include "../include/logica.h"

static SDL_Window *ventana = NULL;
static SDL_Renderer *renderizador = NULL;

static TTF_Font *fuente_costo = NULL;   // contb.ttf

static SDL_Texture *fondo1 = NULL; 
static SDL_Texture *fondoSelector = NULL;     
static SDL_Texture *girasol_textura0 = NULL;
static SDL_Texture *lanzaguisantes_textura0 = NULL;
static SDL_Texture *nuez_textura0 = NULL;
static SDL_Texture *nuez_textura1 = NULL;
static SDL_Texture *nuez_textura2 = NULL;
static SDL_Texture *zombi_textura0 = NULL;
static SDL_Texture *flag_zombi_textura0 = NULL;
static SDL_Texture *guisante_textura0 = NULL;

static SDL_Texture *seed_lanzaguisantes = NULL;
static SDL_Texture *seed_girasol = NULL;
static SDL_Texture *seed_nuez = NULL;
static SDL_Texture *seed_selector = NULL;

static SDL_Texture *pala = NULL;
static SDL_Texture *pala_espacio = NULL;


bool inicializarVisuales(void) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software"); 

    if(!SDL_Init(SDL_INIT_VIDEO)){  //Inicialización SDL
        SDL_Log("Error al iniciar SDL: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {  //Inicialización de TTF (texto)
        SDL_Log("Error al iniciar SDL_ttf: %s", SDL_GetError());
        return false;
    }

    if(!SDL_CreateWindowAndRenderer(NOMBRE_VENTANA, (int)ANCHO, (int)ALTO, 0, &ventana, &renderizador)){    //Creación de ventana
        SDL_Log("Error en ventana: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderizador, SDL_BLENDMODE_BLEND);

    SDL_SetRenderLogicalPresentation(           //Sección para que cuando se maximice la ventana, se autoajuste el contenido.
        renderizador,
        ANCHO,
        ALTO,
        SDL_LOGICAL_PRESENTATION_LETTERBOX
    );

    //Carga de la fuente:
    fuente_costo = TTF_OpenFont("assets/fonts/contb.ttf", 24); // Tamaño 24
    if (!fuente_costo) {
        SDL_Log("Error al cargar la fuente: %s", SDL_GetError());
        return false;
    }

    //Carga de texturas:
    fondo1 = cargarTextura("assets/images/fondo1.jpg");
    if(!fondo1){
        return false;
    }
    fondoSelector = cargarTextura("assets/images/fondoSelector.png");
    if(!fondoSelector){
        return false;
    }
    lanzaguisantes_textura0 = cargarTextura("assets/images/lanzaguisantes_textura0.png");
    if(!lanzaguisantes_textura0){
        return false;
    }
    girasol_textura0 = cargarTextura("assets/images/girasol_textura0.png");
    if(!girasol_textura0){
        return false;
    }
    nuez_textura0 = cargarTextura("assets/images/nuez_textura0.png");
    if(!nuez_textura0){
        return false;
    }
    nuez_textura1 = cargarTextura("assets/images/nuez_textura1.png");
    if(!nuez_textura1){
        return false;
    }
    nuez_textura2 = cargarTextura("assets/images/nuez_textura2.png");
    if(!nuez_textura2){
        return false;
    }
    zombi_textura0 = cargarTextura("assets/images/zombi_textura0.png");
    if(!zombi_textura0){
        return false;
    }
    flag_zombi_textura0 = cargarTextura("assets/images/flag_zombi_textura0.png");
    if(!flag_zombi_textura0){
        return false;
    }
    guisante_textura0 = cargarTextura("assets/images/guisante_textura0.png");
    if(!guisante_textura0){
        return false;
    }
    seed_lanzaguisantes = cargarTextura("assets/images/seed_lanzaguisantes.png");
    if(!seed_lanzaguisantes){
        return false;
    }
    seed_girasol = cargarTextura("assets/images/seed_girasol.png");
    if(!seed_girasol){
        return false;
    }
    seed_nuez = cargarTextura("assets/images/seed_nuez.png");
    if(!seed_nuez){
        return false;
    }
    seed_selector = cargarTextura("assets/images/seed_selector.png");
    if(!seed_selector){
        return false;
    }
    pala = cargarTextura("assets/images/pala.png");
    if(!pala){
        return false;
    }
    pala_espacio = cargarTextura("assets/images/pala_espacio.png");
    if(!pala_espacio){
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
        1100.0f,
        816.0f
    };

    SDL_FRect rect_destino = {
        0.0f,
        0.0f,
        ANCHO,
        ALTO
    };

    SDL_RenderTexture(renderizador, fondo1, &rect_origen, &rect_destino);
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
    if (fuente_costo) {
        TTF_CloseFont(fuente_costo);
        fuente_costo = NULL;
    }
    if(lanzaguisantes_textura0) {
        SDL_DestroyTexture(lanzaguisantes_textura0); 
        lanzaguisantes_textura0 = NULL;
    }
    if(girasol_textura0) {
        SDL_DestroyTexture(girasol_textura0); 
        girasol_textura0 = NULL;
    }
    if(nuez_textura0) {
        SDL_DestroyTexture(nuez_textura0); 
        nuez_textura0 = NULL;
    }
    if(nuez_textura1) {
        SDL_DestroyTexture(nuez_textura1); 
        nuez_textura1 = NULL;
    }
    if(nuez_textura2) {
        SDL_DestroyTexture(nuez_textura2); 
        nuez_textura2 = NULL;
    }
    if(zombi_textura0) {
        SDL_DestroyTexture(zombi_textura0); 
        zombi_textura0 = NULL;
    }
    if(guisante_textura0) {
        SDL_DestroyTexture(guisante_textura0); 
        guisante_textura0 = NULL;
    }
    if(renderizador) SDL_DestroyRenderer(renderizador);
    if(ventana) SDL_DestroyWindow(ventana);
    TTF_Quit();
    MIX_Quit();
    SDL_Quit();
}

void loopVisuales(float pos_camara_x, int estadoJuego, int planta_elegida){
    prepararFondo();

    if(fondo1) {
        RenderizarFondo(renderizador, fondo1, pos_camara_x);
    }

    for (int fila = 0;fila < FILAS_JUEGO;fila++) {
        for (int columna = 0; columna < COLUMNAS_JUEGO;columna++){    

            int elemento = obtener_celda(fila, columna);

            SDL_FRect rectangulo;
            if (elemento == ZOMBIE || elemento == FLAG_ZOMBIE){ //Con esto hago la distinción, los zombis son más altos que las plantas
                float salida_zombis = 90.0;
                rectangulo.x = (float)((MARGEN_X + columna * ANCHO_CELDA) - salida_zombis);
                rectangulo.y = (float)((MARGEN_Y + fila * ALTO_CELDA) - salida_zombis);
                rectangulo.w = (float)ANCHO_CELDA + salida_zombis;
                rectangulo.h = (float)ALTO_CELDA + salida_zombis;
            } else {
                rectangulo.x = (float)(MARGEN_X + columna * ANCHO_CELDA);
                rectangulo.y = (float)(MARGEN_Y + fila * ALTO_CELDA);
                rectangulo.w = (float)ANCHO_CELDA-15;
                rectangulo.h = (float)ALTO_CELDA-15;
            }

            imprimirTextura(elemento, rectangulo, obtener_dano(fila, columna));
            int guisante = obtener_guisante(fila,columna);

            if(guisante){
                SDL_FRect rectanguloGuisante = {
                    (float)(MARGEN_X + columna * ANCHO_CELDA),
                    (float)(MARGEN_Y + fila * ALTO_CELDA),
                    (float)ANCHO_CELDA-15,
                    (float)ALTO_CELDA-15
                };
                imprimirTextura(GUISANTE, rectanguloGuisante, obtener_dano(fila, columna));
            }
        }
    }

    
    
    dibujarUI(planta_elegida);
    dibujarSoles(obtener_soles());

    if(estadoJuego == 3){   //Pantalla oscurecida en la pausa.
        fondoPausa();
    }

    SDL_RenderPresent(renderizador);
    SDL_Delay(16);
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

void imprimirTextura(int planta, SDL_FRect rectangulo, int daño_nuez){
    switch (planta) {
        case GIRASOL:
            SDL_RenderTexture(renderizador, girasol_textura0, NULL, &rectangulo);
            break;
        case LANZAGUISANTES:
            SDL_RenderTexture(renderizador, lanzaguisantes_textura0, NULL, &rectangulo);
            break;
        case NUEZ:
            if (daño_nuez == 0){
                SDL_RenderTexture(renderizador, nuez_textura0, NULL, &rectangulo);  //Textura base
            } else if (daño_nuez == 1) {
                SDL_RenderTexture(renderizador, nuez_textura1, NULL, &rectangulo);  //Textura con daño
            } else {
                SDL_RenderTexture(renderizador, nuez_textura2, NULL, &rectangulo);  //Textura con aún más daño
            }
            
            break;
        case ZOMBIE:
            SDL_RenderTexture(renderizador, zombi_textura0, NULL, &rectangulo);
            break;
        case FLAG_ZOMBIE:
            SDL_RenderTexture(renderizador, flag_zombi_textura0, NULL, &rectangulo);
            break;
        case GUISANTE:
            SDL_RenderTexture(renderizador, guisante_textura0, NULL, &rectangulo);
            break;
    }
}

void fondoPausa(){
    SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 128);
    SDL_FRect pantalla = {0.0f, 0.0f, ANCHO, ALTO};
    SDL_RenderFillRect(renderizador, &pantalla);
}

void dibujarUI(int planta_elegida){
    SDL_FRect rectangulo = {
        MARGEN_X_SELECTOR,
        MARGEN_Y_SELECTOR,
        720.0f,
        100.0f
    };
    SDL_RenderTexture(renderizador, fondoSelector, NULL, &rectangulo);
    
    SDL_FRect rectanguloGirasol = {
        MARGEN_X_SELECTOR + 115,
        MARGEN_Y_SELECTOR + 10,
        60.0f,
        80.0f
    };
    SDL_RenderTexture(renderizador, seed_girasol, NULL, &rectanguloGirasol);

    SDL_FRect rectanguloLanzaguisantes = {
        MARGEN_X_SELECTOR + 115 + 75,
        MARGEN_Y_SELECTOR + 10,
        60.0f,
        80.0f
    };
    SDL_RenderTexture(renderizador, seed_lanzaguisantes, NULL, &rectanguloLanzaguisantes);

    SDL_FRect rectanguloNuez = {
        MARGEN_X_SELECTOR + 115 + 75 + 75,
        MARGEN_Y_SELECTOR + 10,
        60.0f,
        80.0f
    };
    SDL_RenderTexture(renderizador, seed_nuez, NULL, &rectanguloNuez);

    SDL_FRect rectanguloPala = {
        MARGEN_X_SELECTOR + 720.0f,
        MARGEN_Y_SELECTOR,
        80.0f,
        80.0f
    };
    SDL_RenderTexture(renderizador, pala_espacio, NULL, &rectanguloPala);
    int pala_seleccionada = 0;
    switch (planta_elegida){
        case GIRASOL:
            SDL_RenderTexture(renderizador, seed_selector, NULL, &rectanguloGirasol);
            break;
        case LANZAGUISANTES:
            SDL_RenderTexture(renderizador, seed_selector, NULL, &rectanguloLanzaguisantes);
            break;
        case NUEZ:
            SDL_RenderTexture(renderizador, seed_selector, NULL, &rectanguloNuez);
            break;
        case PALA:
            pala_seleccionada = 1;
    }   
    
    if(!pala_seleccionada){
        SDL_RenderTexture(renderizador, pala, NULL, &rectanguloPala);
    }

    }

void dibujarSoles(int cantidad_soles) { 
    if (!fuente_costo) return;

    // Convertimos el número entero a string
    char texto_soles[16];
    snprintf(texto_soles, sizeof(texto_soles), "%d", cantidad_soles);

    // Color del texto
    SDL_Color color_texto = {0, 0, 0, 255};

   // Superficie del texto
    SDL_Surface *superficie_texto = TTF_RenderText_Blended(fuente_costo, texto_soles, 0, color_texto);
    if (!superficie_texto) return;
    
    float ancho_texto = (float)superficie_texto->w;
    float alto_texto = (float)superficie_texto->h;

    SDL_Texture *textura_texto = SDL_CreateTextureFromSurface(renderizador, superficie_texto);

    SDL_DestroySurface(superficie_texto);

    if (!textura_texto) return;

    
    // Rectángulo de salida
    SDL_FRect rect_destino = {
        (float)(147.0f - ancho_texto)/2,  // Posición X 
        (float)(170.0f - alto_texto)/2,  // Posición Y
        (float)ancho_texto, // Ancho de la textura generada
        (float)alto_texto  // Alto de la textura generada
    };

    // Renderizar texto
    SDL_RenderTexture(renderizador, textura_texto, NULL, &rect_destino);

    SDL_DestroyTexture(textura_texto);
}

void pantallaDerrota(){

}

void pantallaVictoria(){

}