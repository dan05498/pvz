#include <stdio.h>
#include "confi.h"

int main(){
    struct configuracion cfg;
    int mapa[8][13];
    int filas, columnas;

    //funciones:
    cargar_configuracion(&cfg, "../config/config.txt");
    cargar_mapa(mapa, &filas, &columnas, "../include/mapa.txt");
    //poner el resto de cosas los includes arriba para mas orden este archivo lo traje para no modificar el codigo de prueba

    return 0;
}
