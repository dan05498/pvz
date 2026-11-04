/*
 * logica.c
 * Cmo funciona:
 *  - El juego avanza por "turnos". Cada vez que se llama a actualizar_juego()
 *    pasa un turno.
 *  - La columna 0 es la casa. Los zombies aparecen en la última columna y
 *    caminan hacia la izquierda.
 *  - Cada celda de la matriz guarda UNA sola cosa (una planta, un zombie,
 *    un guisante, pasto...).
 *  - Para saber cuánta vida le queda a cada planta o zombie usamos una
 *    segunda matriz llamada "vida", del mismo tamaño que el mapa.
 */

#include <stdlib.h>
#include <time.h>
#include "logica.h"

// CONFIG INICIAL DEL JUEGO (modificable)

#define TOTAL_OLEADAS         5
#define VIDA_CASA_INICIAL     6    // golpes de zombie que aguanta la casa 
#define VIDA_ZOMBIE           6    // guisantes que aguanta un zombie  
#define VIDA_NUEZ             5    // golpes de zombie que aguanta la nuez 
#define VIDA_PLANTA           1    // girasol y lanzaguisantes: 1 golpe 

#define SOLES_INICIALES       150
#define COSTO_LANZAGUISANTES  100   
#define COSTO_GIRASOL         50
#define COSTO_NUEZ            150  

#define SOLES_AUTOMATICO      25   // soles que caen solos
#define SOLES_GIRASOL         25   // soles que produce un girasol  

// "CADA_X" significa: esto ocurre cada X turnos. 
#define CADA_SOL_AUTOMATICO   12
#define CADA_SOL_GIRASOL      8
#define CADA_DISPARO          2
#define CADA_PASO_ZOMBIE      3
#define CADA_PASO_FLAG        2    // el flag zombie es más rápido 
#define CADA_ZOMBIE_NUEVO     4    // separación entre zombies de una oleada 

#define TURNOS_PREPARACION    30   // turnos sin zombies al comienzo 
#define TURNOS_DESCANSO       15   // turnos entre una oleada y otra 

// Las tres fases de la partida
#define FASE_PREPARACION      0
#define FASE_OLEADA           1
#define FASE_DESCANSO         2


//VARIABLES


static int **vida;          //matriz q guarda la vida de cada elemento
static int filas_juego;
static int columnas_juego;

static int estado;          // jugando, visctoria o derrota 
static int fase;            // si está en FASE_PREPARACION, FASE_OLEADA o FASE_DESCANSO 
static int turno;
static int soles;
static int vida_casa;
static int oleada;          // número de la oleada actual (0 = aún no empieza) 
static int espera;          // turnos que faltan para que termine preparación/descanso */
static int zombies_por_salir;   // zombies de la oleada que aún no aparecen 
static int bandera_por_salir;   // 1 si falta que aparezca el flag zombie 


//FUNCIONES AUXILIARES


// Devuelve 1 si el número es una planta, 0 si no. 
static int es_planta(int codigo) {  //para pegar, pasar los guisantes, etc
    return codigo == GIRASOL || codigo == LANZAGUISANTES || codigo == NUEZ;
}

// Devuelve 1 si el número es un zombie, 0 si no. 
static int es_zombie(int codigo) {    //para golpear el guisante, para pasar por encima si no es etc
    return codigo == ZOMBIE || codigo == FLAG_ZOMBIE;
}

// Cuánta vida tiene al empezar lo que hay en una celda. 
static int vida_inicial(int codigo) {
    if (codigo == NUEZ) {
        return VIDA_NUEZ;
    }
    if (codigo == GIRASOL || codigo == LANZAGUISANTES) {
        return VIDA_PLANTA;
    }
    if (es_zombie(codigo)) {
        return VIDA_ZOMBIE;
    }
    return 0;
}

//Crea una matriz dinámica de filas x columnas con todo en 0.
static int **crear_matriz_vida(int filas, int columnas) {
    int **m = malloc(filas * sizeof(int *));
    for (int f = 0; f < filas; f++) {
        m[f] = malloc(columnas * sizeof(int));
        for (int c = 0; c < columnas; c++) {
            m[f][c] = 0;
        }
    }
    return m;
}

/* Le quita 1 de vida a lo que haya en la celda (f, c).
 * Si la vida llega a 0, la celda vuelve a ser pasto. */
static void quitar_vida(int **mapa, int f, int c) {
    vida[f][c]--;
    if (vida[f][c] <= 0) {
        vida[f][c] = 0;
        mapa[f][c] = PASTO;
    }
}

// Cuenta cuántos zombies hay en el jardín, para saber si termino la oleada
static int contar_zombies(int **mapa) {
    int total = 0;
    for (int f = 0; f < filas_juego; f++) {
        for (int c = 0; c < columnas_juego; c++) {
            if (es_zombie(mapa[f][c])) {
                total++;
            }
        }
    }
    return total;
}

// Devuelve 1 si hay algún zombie a la derecha de la celda (f, c).
static int hay_zombie_a_la_derecha(int **mapa, int f, int c) {
    for (int k = c + 1; k < columnas_juego; k++) {
        if (es_zombie(mapa[f][k])) {
            return 1;
        }
    }
    return 0;
}

//INICIO Y FIN DE LA LÓGICA

void iniciar_logica(int **mapa, int filas, int columnas) {
    filas_juego = filas;
    columnas_juego = columnas;
    vida = crear_matriz_vida(filas, columnas);

    /* La columna 0 es la casa. Además, si el mapa ya trae plantas o
     * zombies, les ponemos su vida inicial. */
    for (int f = 0; f < filas; f++) {
        mapa[f][0] = CASA;
        for (int c = 1; c < columnas; c++) {
            vida[f][c] = vida_inicial(mapa[f][c]);
        }
    }

    estado = JUGANDO;
    fase = FASE_PREPARACION;
    turno = 0;
    soles = SOLES_INICIALES;
    vida_casa = VIDA_CASA_INICIAL;
    oleada = 0;
    espera = TURNOS_PREPARACION;
    zombies_por_salir = 0;
    bandera_por_salir = 0;

    srand((unsigned)time(NULL));   // para que los zombies salgan por filas al azar 
}

void liberar_logica(void) {
    for (int f = 0; f < filas_juego; f++) {
        free(vida[f]);
    }
    free(vida);
    vida = NULL;
}


//FUNCIONES PARA PLANTAR


int costo_planta(int tipo) {
    if (tipo == LANZAGUISANTES) {
        return COSTO_LANZAGUISANTES;
    }
    if (tipo == GIRASOL) {
        return COSTO_GIRASOL;
    }
    if (tipo == NUEZ) {
        return COSTO_NUEZ;
    }
    return 0;
}

int plantar(int **mapa, int tipo, int fila, int col) {
    if (estado != JUGANDO || !es_planta(tipo)) {  //para plantar en la fase de jugar
        return 0;
    }
    if (fila < 0 || fila >= filas_juego) { //para q quede en las filas del juego
        return 0;
    }
    /* No se planta en la casa (columna 0) ni en la última columna,
     * porque por ahí aparecen los zombies. */
    if (col < 1 || col > columnas_juego - 2) {
        return 0;
    }
    if (mapa[fila][col] != PASTO) { //para que no se plante en nada que no sea pasto
        return 0;
    }
    if (soles < costo_planta(tipo)) {   //para que no compre sin tener los soles suficientes
        return 0;
    }

    soles -= costo_planta(tipo);   //resta los soles
    mapa[fila][col] = tipo;		//la planta en el mapa
    vida[fila][col] = vida_inicial(tipo); //le da la vida a la matriz vida
    return 1;
}


//FUNCION APARICION DE SOLES


static void generar_soles(int **mapa) {
    /* Sol automático: cae solo cada cierto tiempo */
    if (turno % CADA_SOL_AUTOMATICO == 0) {  //si pasaron un multiplo de turnos de sol automatico
        soles += SOLES_AUTOMATICO;     //dropea soles
    }

    /* Girasoles: cada uno produce soles cada cierto tiempo */
    if (turno % CADA_SOL_GIRASOL == 0) {    //si pasa mult de turnos de sol girasol
        for (int f = 0; f < filas_juego; f++) {
            for (int c = 1; c < columnas_juego; c++) {
                if (mapa[f][c] == GIRASOL) {
                    soles += SOLES_GIRASOL;    //suma soles cada x girasol
                }
            }
        }
    }
}


//FUNCIONES GUISANTES

/* Un guisante llega a la celda (f, col). depende de lo que haya ahí:
 *  - plantas:  el guisante pasa por encima y sigue a la celda siguiente
 *  - zombie:   el zombie pierde 1 de vida y el guisante desaparece
 *  - pasto:    el guisante se queda ahí (se ve como un GUISANTE en el mapa)
 *  - fuera del jardín: el guisante desaparece */
static void llegar_guisante(int **mapa, int f, int col) {
    while (col < columnas_juego && es_planta(mapa[f][col])) { //mientras q esté en las columnas o haya una planta avanza
        col++;
    }
    if (col >= columnas_juego) {     //si se pasa desaparece
        return;
    }
    if (es_zombie(mapa[f][col])) {
        quitar_vida(mapa, f, col);    //si choca con un zombie le resta vida
    } else if (mapa[f][col] == PASTO) {
        mapa[f][col] = GUISANTE;
    }
}

// Cada guisante avanza una celda hacia la derecha.
static void mover_guisantes(int **mapa) {
    for (int f = 0; f < filas_juego; f++) {
        for (int c = columnas_juego - 1; c >= 1; c--) {
            if (mapa[f][c] == GUISANTE) {
                mapa[f][c] = PASTO;               /* sale de esta celda */
                llegar_guisante(mapa, f, c + 1);  /* y llega a la siguiente */
            }
        }
    }
}

//Los lanzaguisantes disparan, pero solo si hay un zombie en su fila. 
static void disparar_plantas(int **mapa) {
    if (turno % CADA_DISPARO != 0) {
        return;
    }
    for (int f = 0; f < filas_juego; f++) {
        for (int c = 1; c < columnas_juego; c++) {
            if (mapa[f][c] == LANZAGUISANTES && hay_zombie_a_la_derecha(mapa, f, c)) {
                llegar_guisante(mapa, f, c + 1);  /* el guisante nace justo a su derecha */
            }
        }
    }
}

//FUNCIONES ZOMBIES

/* Cada zombie, cuando le toca actuar, mira la celda que tiene a su izquierda:
 *  - la casa:  la golpea (la casa pierde 1 de vida y ese zombie desaparece)
 *  - una planta: la golpea (la planta pierde 1 de vida)
 *  - pasto:    avanza una celda
 *  - otra cosa (otro zombie, un guisante): espera */
static void mover_zombies(int **mapa) {
    for (int f = 0; f < filas_juego; f++) {
        for (int c = 1; c < columnas_juego; c++) {
            int tipo = mapa[f][c];
            if (!es_zombie(tipo)) {
                continue;
            }
            int cada = CADA_PASO_ZOMBIE;
            if (tipo == FLAG_ZOMBIE) {
                cada = CADA_PASO_FLAG;  //avanza dependiendo del tipo de zombie q sea
            }
            if (turno % cada != 0) {
                continue;
            }

            int delante = mapa[f][c - 1];  //delante es lo q tiene a la izq
            if (delante == CASA) {
                vida_casa--;   //le pega a la casa y desaparece
                mapa[f][c] = PASTO;
                vida[f][c] = 0;
            } else if (es_planta(delante)) {
                quitar_vida(mapa, f, c - 1);  //le quita vida al guisante
            } else if (delante == PASTO) {
                mapa[f][c - 1] = tipo;
                vida[f][c - 1] = vida[f][c]; //si es paso avanza el zombie, avanza con su vida
                mapa[f][c] = PASTO;   //la celda en la que estaba se hace pasto con vida 0
                vida[f][c] = 0;
            }
        }
    }
}

// FUNCIONES OLEADAS 

/* Hace aparecer un zombie en la última columna, en una fila al azar.
 * Si esa celda está ocupada no hace nada y devuelve 0 (se intenta de nuevo
 * más tarde). Devuelve 1 si el zombie apareció. */
static int aparecer_zombie(int **mapa) {
    int f = rand() % filas_juego;
    int ultima = columnas_juego - 1;

    if (mapa[f][ultima] != PASTO) {    //si no es pasto no aparece
        return 0;
    }
    if (bandera_por_salir) {
        mapa[f][ultima] = FLAG_ZOMBIE;     //si es que debe aparecer un flag
        bandera_por_salir = 0;
    } else {
        mapa[f][ultima] = ZOMBIE;    //aparece el zombie
    }
    vida[f][ultima] = VIDA_ZOMBIE;   //se pone su vida en la matriz de vida
    return 1;
}

/* Prepara una nueva oleada. Cada oleada trae más zombies que la anterior.
 * Las oleadas 3, 6, y la última traen además un flag zombie. */
static void empezar_oleada(void) {
    oleada++;
    fase = FASE_OLEADA;
    zombies_por_salir = 3 * oleada + 2;   //mosificable, pero eso se me ocurrió para sumar zombies a la oleada xd
    bandera_por_salir = 0;

    if (oleada % 3 == 0 || oleada == TOTAL_OLEADAS) {
        zombies_por_salir += 3; //oleada 3,6 y así son más difíciles, vienen con más zombies
        bandera_por_salir = 1;   //y un flag
    }
}

// hace oleada, preparación, oleada, preparación 
static void controlar_oleadas(int **mapa) {
    if (fase == FASE_PREPARACION || fase == FASE_DESCANSO) {
        espera--;
        if (espera <= 0) {		//si no estamos en prep o descanso, empieza la oleada
            empezar_oleada();
        }
    } else if (fase == FASE_OLEADA) {
        if (zombies_por_salir > 0) { //si faltan zomb por salir spawneamos
            if (turno % CADA_ZOMBIE_NUEVO == 0 && aparecer_zombie(mapa)) { //si spawnea bien restamos un zombie al cont
                zombies_por_salir--;   
            }
        } else if (contar_zombies(mapa) == 0) {  //si no quedan zombies
            if (oleada >= TOTAL_OLEADAS) { //y no quedan oleadas ganamos
                estado = VICTORIA;
            } else {
                fase = FASE_DESCANSO;  //si no pasamos a la sigueente oleada
                espera = TURNOS_DESCANSO; //y descansamos
            }
        }
    }
}


//  FUNCIÓN PRINCIPAL 

void actualizar_juego(int **mapa) {
    if (estado != JUGANDO) {
        return;
    }

    turno++;
    generar_soles(mapa);   //realiza las acciones principales
    mover_guisantes(mapa);
    disparar_plantas(mapa);
    mover_zombies(mapa);

    if (vida_casa <= 0) {
        vida_casa = 0;
        estado = DERROTA; //si la casa llega a 0 perdemos
        return;
    }
    controlar_oleadas(mapa); //controlamos oleadas
}


// datos y funciones para la visaulización (modificable)

int estado_juego(void) {
    return estado;
}

int obtener_soles(void) {
    return soles;
}

int obtener_vida_casa(void) {
    return vida_casa;
}

int obtener_oleada(void) {
    return oleada;
}

int obtener_total_oleadas(void) {
    return TOTAL_OLEADAS;
}
