#include <stdio.h>
#include "../include/entradas.h"
#include "../include/logica.h"
#include "../include/visuales.h"


int main(){
    struct configuracion cfg;
    int mapa[8][13];
    int filas, columnas;

    //funciones:
    cargar_configuracion(&cfg, "../config/config.txt");
    cargar_mapa(mapa, &filas, &columnas, "../config/mapa.txt");


    //Arranque de Visuales:
    if(!inicializarVisuales()){
        return 1;
    }

    //Carga de texturas:

    //Pasto:
    SDL_Texture *texturaPasto1 = cargarTextura("assets/grass1.png");
    if(!texturaPasto1){
        cerrarVisuales();
        return 1;
    }

    SDL_Texture *texturaPasto2 = cargarTextura("assets/grass2.png");
    if(!texturaPasto2){
        cerrarVisuales();
        return 1;
    }

    bool jugando = true;

    //Ciclo de juego:

    while(jugando){
        
        //Eventos (Entradas)
        SDL_Event evento;
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){  //Botón de cerrar
                jugando = false;
            }
        }

        prepararFondo();

        for(int fila = 0; fila < filas; fila++){
            for(int columna = 0; columna < columnas; columna++){
                if(mapa[fila][columna] == 0){
                    dibujarCasilla(texturaPasto1, fila, columna);
                } else if(mapa[fila][columna] == 1){
                    dibujarCasilla(texturaPasto2, fila, columna);
                }
            }
        }

        presentarPantalla();
    }

    SDL_DestroyTexture(texturaPasto1);
    SDL_DestroyTexture(texturaPasto2);
    cerrarVisuales();

    return 0;
}
