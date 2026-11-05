/* CREE ESTE ARCHIVO CON CLAUDE PARA PROBAR MI LÓGICA DEL JUEGO
 * main_prueba.c
 * Programa TEMPORAL para probar la lógica (Tarea 2) en la consola, sin
 * depender de la lectura de archivos (Tarea 1) ni de la visualización
 * (Tarea 3). Cuando el grupo junte todo, este archivo se reemplaza por el
 * main real.
 *
 * Aquí hacemos "a mano" lo que después hará la Tarea 1 (crear la matriz) y
 * la Tarea 3 (mostrarla en pantalla).
 */

#include <stdio.h>
#include <stdlib.h>
#include "include/logica.h"

#define FILAS     5
#define COLUMNAS  10

/* Crea la matriz del jardín: columna 0 = casa, el resto = pasto.
 * (Esto lo hará la Tarea 1 en el proyecto final.) */
static int **crear_mapa(int filas, int columnas) {
    int **mapa = malloc(filas * sizeof(int *));
    for (int f = 0; f < filas; f++) {
        mapa[f] = malloc(columnas * sizeof(int));
        for (int c = 0; c < columnas; c++) {
            mapa[f][c] = PASTO;
        }
        mapa[f][0] = CASA;
    }
    return mapa;
}

static void liberar_mapa(int **mapa, int filas) {
    for (int f = 0; f < filas; f++) {
        free(mapa[f]);
    }
    free(mapa);
}

/* Letra que se imprime para cada número de la matriz */
static char simbolo(int codigo) {
    if (codigo == PASTO)          return '.';
    if (codigo == CASA)           return 'H';
    if (codigo == LAPIDA)         return '#';
    if (codigo == GIRASOL)        return 'G';
    if (codigo == LANZAGUISANTES) return 'L';
    if (codigo == NUEZ)           return 'N';
    if (codigo == ZOMBIE)         return 'Z';
    if (codigo == FLAG_ZOMBIE)    return 'B';
    if (codigo == GUISANTE)       return '*';
    return '?';
}

/* Dibuja el jardín y los datos del juego.
 * (Esto lo hará la Tarea 3 en el proyecto final.) */
static void imprimir_juego(int **mapa) {
    printf("\nOleada %d/%d | Soles: %d | Vida casa: %d\n",
           obtener_oleada(), obtener_total_oleadas(),
           obtener_soles(), obtener_vida_casa());

    for (int f = 0; f < FILAS; f++) {
        printf("%d  ", f);
        for (int c = 0; c < COLUMNAS; c++) {
            printf("%c ", simbolo(mapa[f][c]));
        }
        printf("\n");
    }
}

int main(void) {
    int **mapa = crear_mapa(FILAS, COLUMNAS);
    iniciar_logica(mapa, FILAS, COLUMNAS);

    while (estado_juego() == JUGANDO) {
        imprimir_juego(mapa);
        printf("1=girasol (%d)  2=lanzaguisantes (%d)  3=nuez (%d)\n",
               costo_planta(GIRASOL), costo_planta(LANZAGUISANTES), costo_planta(NUEZ));
        printf("0=pasar turno  9=avanzar 10 turnos\n> ");

        int opcion;
        if (scanf("%d", &opcion) != 1) {
            break;
        }

        if (opcion >= 1 && opcion <= 3) {
            /* Los números 1, 2 y 3 coinciden con GIRASOL, LANZAGUISANTES y NUEZ */
            int fila, col;
            printf("fila y columna: ");
            if (scanf("%d %d", &fila, &col) != 2) {
                break;
            }
            if (!plantar(mapa, opcion, fila, col)) {
                printf("No se pudo plantar (pocos soles, celda ocupada o fuera de rango)\n");
            }
        } else if (opcion == 0) {
            actualizar_juego(mapa);
        } else if (opcion == 9) {
            for (int i = 0; i < 10 && estado_juego() == JUGANDO; i++) {
                actualizar_juego(mapa);
            }
        }
    }

    imprimir_juego(mapa);
    if (estado_juego() == VICTORIA) {
        printf("\n¡GANASTE! Sobreviviste todas las oleadas.\n");
    } else if (estado_juego() == DERROTA) {
        printf("\nPERDISTE: los zombies destruyeron la casa.\n");
    }

    liberar_logica();
    liberar_mapa(mapa, FILAS);
    return 0;
}
