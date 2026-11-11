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

    int mapa[9][11] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }

    //funciones:
    // cargar_configuracion(&cfg, "../config/config.txt");
    // cargar_mapa(mapa, &filas, &columnas, "../config/mapa.txt");

    if(!audio_iniciar()) return 1;

   /*para que funcione:
cd ~/pvz/pvz
./compilar.sh
cd src
../pvz
*/


    //Arranque de Visuales:
    if(!inicializarVisuales()){
        return 1;
    }

    float pos_camara_x = 500.0f;

    //Ciclo de juego:
    bool jugando = true;
    estado estadoJuego = INTRO;

    while(jugando){

        //Eventos (Entradas)
        SDL_Event evento;
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){  //Botón de cerrar
                jugando = false;
            }
        }

        switch (estadoJuego) {
            case MENU:
                
                break;
            case INTRO:
                if (!animacionInicio(&pos_camara_x)){
                    estadoJuego = PLAY;    
                }
                loopVisuales(pos_camara_x);
                break;
            case PLAY:



                
                break;
        }

        loopVisuales(pos_camara_x);
    }

    cerrarSDL();

    return 0;
}
