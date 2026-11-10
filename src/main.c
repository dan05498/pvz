#include <stdio.h>
#include "../include/entradas.h"
#include "../include/logica.h"
#include "../include/visuales.h"
   #include "../include/audio.h"


int main(){
    struct configuracion cfg;
    int mapa[8][13];
    int filas, columnas;

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

    bool inicio = true;
    float pos_camara_x = 500.0f;
    float pos_camara_jugando = 210.0f;
    float vel_camara = 4.0f;

    //Animación al inicio (desde la calle hasta la cuadricula)
    while(inicio){
        if (!animacionInicio(&pos_camara_x, pos_camara_jugando, vel_camara)){
            inicio = false;
        }
        loopVisuales(pos_camara_x);
    }

    //Ciclo de juego:
    bool jugando = true;

    while(jugando){
        
        //Eventos (Entradas)
        SDL_Event evento;
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){  //Botón de cerrar
                jugando = false;
            }
        }

        loopVisuales(pos_camara_x);
    }

    cerrarSDL();

    return 0;
}
