#include <stdio.h>
#include "confi.h"

//ahora se carga la confi
void cargar_configuracion(struct configuracion *cfg, const char *ruta){
    FILE *archivo_cfg = fopen(ruta, "r");
    if(archivo_cfg == NULL){
        printf("no se cargo la configuracion\n");
        return; //el return vacio por que es void
    }

    fscanf(archivo_cfg, "%d", &(*cfg).TOTAL_OLEADAS);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_CASA_INICIAL);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_ZOMBIE);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_NUEZ);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_GIRASOL);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_LANZAGUISANTES);
    fscanf(archivo_cfg, "%d", &(*cfg).SOLES_INICIALES);
    fscanf(archivo_cfg, "%d", &(*cfg).COSTO_LANZAGUISANTES);
    fscanf(archivo_cfg, "%d", &(*cfg).COSTO_GIRASOL);
    fscanf(archivo_cfg, "%d", &(*cfg).COSTO_NUEZ);
    fscanf(archivo_cfg, "%d", &(*cfg).SOLES_AUTOMATICO);
    fscanf(archivo_cfg, "%d", &(*cfg).SOLES_GIRASOL);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_SOL_AUTOMATICO);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_SOL_GIRASOL);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_DISPARO);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_PASO_ZOMBIE);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_PASO_FLAG);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_ZOMBIE_NUEVO);
    fscanf(archivo_cfg, "%d", &(*cfg).TURNOS_PREPARACION);
    fscanf(archivo_cfg, "%d", &(*cfg).TURNOS_DESCANSO);
    fscanf(archivo_cfg, "%d", &(*cfg).FASE_PREPARACION);
    fscanf(archivo_cfg, "%d", &(*cfg).FASE_OLEADA);
    fscanf(archivo_cfg, "%d", &(*cfg).FASE_DESCANSO);
    

    fclose(archivo_cfg); //cerramos el archivo para liberar la memoria
    
}

//cargar el mapa en el tablero del juego 

 void cargar_mapa(int matriz[8][13], int *filas, int *columnas, const char *ruta){
    FILE *archivo_mapa = fopen(ruta,"r"); // se abre en modo lectura
    if(archivo_mapa == NULL){
        printf("no se cargo el mapa"); //esto puede deberse a la ruta normalmente
        return; //void
    }

    fscanf(archivo_mapa, "%d %d", filas, columnas);

    for(int i=0; i<*filas;i++){//asi nos basamos en el puntero
        for(int j=0;j<*columnas;j++){
            fscanf(archivo_mapa, "%d", &matriz[i][j]);
        }
    }

    fclose(archivo_mapa);
 }
