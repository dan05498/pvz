#include <stdio.h>
#include <stdlib.h>
#include "../include/entradas.h"
#include "../include/logica.h"

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
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_LANZAHIELO);
    fscanf(archivo_cfg, "%d", &(*cfg).COSTO_LANZAHIELO);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_DISPARO_HIELO);
    fscanf(archivo_cfg, "%d", &(*cfg).TURNOS_CONGELADO);
    fscanf(archivo_cfg, "%d", &(*cfg).VIDA_GIGANTE);
    fscanf(archivo_cfg, "%d", &(*cfg).DANO_GIGANTE);
    fscanf(archivo_cfg, "%d", &(*cfg).CADA_PASO_GIGANTE);
    fscanf(archivo_cfg, "%d", &(*cfg).ESPERA_PLANTAR);
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

void generar_tumbas(int mapa[5][11], const char *tumbass){
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 0; c < COLUMNAS_JUEGO; c++) {
            if (mapa[f][c] == TUMBAS) {
                mapa[f][c] = PASTO;
            }
        }
    }

    int tumbas = 6;
    while(tumbas>0){
        int f = rand() % FILAS_JUEGO;           // cualquier fila
        int c = 1 + rand() % (COLUMNAS_JUEGO - 2);      // columnas de la 1 a COLUMNAS_JUEGO-2 para evitar generar tumbas en donde esta la generacion de zombies o en la casa
        if (mapa[f][c] == PASTO) {            // solo en celdas libres
            mapa[f][c] = TUMBAS;                   //el numero de la tumba es 8
            tumbas--;
        }
    }

    FILE *archivo_mapa_tumbas = fopen("config/mapa.txt", "w"); //hacemos el archivo de guardado con w que es write para escribir los datos que llevamos

    for(int i=0; i<FILAS_JUEGO; i++){
    for(int j=0; j<COLUMNAS_JUEGO; j++){
        fprintf(archivo_mapa_tumbas, "%d ", obtener_celda(i,j));
        }
        fprintf(archivo_mapa_tumbas, "\n");
        }
    fclose(archivo_mapa_tumbas);
}
