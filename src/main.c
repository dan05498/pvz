#include <stdio.h>
#include "../include/entradas.h"
#include "../include/logica.h"
#include "../include/visuales.h"
#include "../include/audio.h"

typedef enum {
    MENU,
    INTRO,
    PLAY,
    PAUSA,
    DEFEAT,
    VICTORY
} estado;

int main(){
    struct configuracion cfg;

    //funciones:
    cargar_configuracion(&cfg, "config/config.txt");
    iniciar_logica(&cfg);

    if(!audio_iniciar()) return 1;

    //Arranque de Visuales:
    if(!inicializarVisuales()){
        return 1;
    }

    float pos_camara_x = 500.0f;

    //Ciclo de juego:
    bool jugando = true;
    estado estadoJuego = INTRO;

    int planta_elegida = GIRASOL;
    Uint64 ultimo_turno = 0;
    Uint64 ahora = 0;
    Uint64 ms_por_turno = 1000;

    while(jugando){

        //Eventos (Entradas)
        SDL_Event evento;
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){  //Botón de cerrar
                jugando = false;
            }

            if(estadoJuego == PLAY && evento.type == SDL_EVENT_KEY_DOWN){
                if(evento.key.key == SDLK_1){
                    planta_elegida = GIRASOL;
                }
                if(evento.key.key == SDLK_2){
                    planta_elegida = LANZAGUISANTES;
                }
                if(evento.key.key == SDLK_3){
                    planta_elegida = NUEZ;
                }
            }

            if(estadoJuego == PLAY && evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT){
                if(evento.button.x >= MARGEN_X && evento.button.y >= MARGEN_Y){
                    int col = (int)((evento.button.x - MARGEN_X) / ANCHO_CELDA);
                    int fila = (int)((evento.button.y - MARGEN_Y) / ALTO_CELDA);
                    plantar(planta_elegida, fila, col);
                }
            }
        }

        switch (estadoJuego) {
            case MENU:
                
                break;
            case INTRO:
                if (!animacionInicio(&pos_camara_x)){
                    estadoJuego = PLAY;
                    ultimo_turno = SDL_GetTicks();    
                }
                break;
            case PLAY:
                ahora = SDL_GetTicks();
                if(ahora - ultimo_turno >= ms_por_turno){
                    actualizar_juego();
                    ultimo_turno = ahora;
                }

                if(estado_juego() == VICTORIA){
                    printf("Ganaste\n");    //por ahora donde no hay pantalla de victoria xD
                    estadoJuego = VICTORY;
                }
                if(estado_juego() == DERROTA){
                    printf("Perdiste\n");
                    estadoJuego = DEFEAT;
                }



                
                break;
        }

        loopVisuales(pos_camara_x);
    }

    cerrarSDL();

    return 0;
}
