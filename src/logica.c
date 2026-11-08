#include <stdlib.h>
#include <time.h>
#include "../include/logica.h"

#define EN_PREPARACION   0
#define EN_OLEADA        1
#define EN_DESCANSO      2

static struct configuracion config;

static int jardin[FILAS_JUEGO][COLUMNAS_JUEGO];  //Matriz de Juego
static int vida[FILAS_JUEGO][COLUMNAS_JUEGO];   //Matriz con la vida de las cosas del jardín

static int estado;   //jugando, victoria, derrota
static int fase;	//preparacion, oleada, descanso
static int turno;	//turno de la partida
static int soles;
static int vida_casa;
static int oleada;
static int espera;
static int zombies_por_salir;
static int bandera_por_salir;


static int es_planta(int codigo) {  //si es planta el zombie o el guisante toma una decisión
    return codigo == GIRASOL || codigo == LANZAGUISANTES || codigo == NUEZ;
}

static int es_zombie(int codigo) { //Si es zombie el zombie o guisante toma una decisión
    return codigo == ZOMBIE || codigo == FLAG_ZOMBIE;
}

static int vida_inicial(int codigo) {  //Vida de las plantas
    if (codigo == NUEZ) {
        return config.VIDA_NUEZ;
    }
    if (codigo == GIRASOL) {
        return config.VIDA_GIRASOL;
    }
    if (codigo == LANZAGUISANTES) {
        return config.VIDA_LANZAGUISANTES;
    }
    if (es_zombie(codigo)) {
        return config.VIDA_ZOMBIE;
    }
    return 0;
}

static void dejar_pasto(int f, int c) {  //Cada que el zombie o guisante se mueve devolvemos pasto
    jardin[f][c] = PASTO;
    vida[f][c] = 0;
}

static void quitar_vida(int f, int c) {   //quitamos la vida en la matriz de vida
    vida[f][c]--;
    if (vida[f][c] <= 0) {
        dejar_pasto(f, c);
    }
}

static int contar_zombies(void) {   //Contamos zombies para saber cuánto queda de oleada
    int total = 0;
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 0; c < COLUMNAS_JUEGO; c++) {
            if (es_zombie(jardin[f][c])) {
                total++;
            }
        }
    }
    return total;
}

static int hay_zombie_a_la_derecha(int f, int c) {  //sabemos si el lanzaguisante dispara
    for (int k = c + 1; k < COLUMNAS_JUEGO; k++) {
        if (es_zombie(jardin[f][k])) {
            return 1;
        }
    }
    return 0;
}


void iniciar_logica(struct configuracion *cfg) { //Iniciamos la matriz de juego y vida
    config = *cfg;
    for (int f = 0; f < FILAS_JUEGO; f++) {
        jardin[f][0] = CASA;
        vida[f][0] = 0;
        for (int c = 1; c < COLUMNAS_JUEGO; c++) {
            jardin[f][c] = PASTO;
            vida[f][c] = 0;
        }
    }

    estado = JUGANDO;	//iniciamos los indicadores
    fase = EN_PREPARACION;
    turno = 0;
    soles = config.SOLES_INICIALES;
    vida_casa = config.VIDA_CASA_INICIAL;
    oleada = 0;
    espera = config.TURNOS_PREPARACION;
    zombies_por_salir = 0;
    bandera_por_salir = 0;

    srand((unsigned)time(NULL));
}


int costo_planta(int tipo) {  //Retornamos el costo para restarle a nuestros soles
    if (tipo == LANZAGUISANTES) {
        return config.COSTO_LANZAGUISANTES;
    }
    if (tipo == GIRASOL) {
        return config.COSTO_GIRASOL;
    }
    if (tipo == NUEZ) {
        return config.COSTO_NUEZ;
    }
    return 0;
}

int plantar(int tipo, int fila, int col) { //Función para plantar
    if (estado != JUGANDO || !es_planta(tipo)) {  //Restricciones, no tenemos soles, fuera de la matriz
        return 0;							     //estamos intentando plantar donde hay otra planta, etc
    }
    if (fila < 0 || fila >= FILAS_JUEGO) {
        return 0;
    }
    if (col < 1 || col > COLUMNAS_JUEGO - 2) {
        return 0;
    }
    if (jardin[fila][col] != PASTO) {
        return 0;
    }
    if (soles < costo_planta(tipo)) {
        return 0;
    }
    soles -= costo_planta(tipo);   //nos restamos el costo de la planta 
    jardin[fila][col] = tipo;		//ponemos la planta en la matriz
    vida[fila][col] = vida_inicial(tipo);	//ponemos la vida de la planta
    return 1;
}


static void generar_soles(void) {
    if (turno % config.CADA_SOL_AUTOMATICO == 0) {   //se genera sol autom cada X turnos
        soles += config.SOLES_AUTOMATICO;
    }
    if (turno % config.CADA_SOL_GIRASOL == 0) {   //se genera sol de girasol cada X turnos
        for (int f = 0; f < FILAS_JUEGO; f++) {
            for (int c = 1; c < COLUMNAS_JUEGO; c++) {
                if (jardin[f][c] == GIRASOL) {
                    soles += config.SOLES_GIRASOL;
                }
            }
        }
    }
}


static void llegar_guisante(int f, int col) { //función q decide q hará el guisante(avanzar, golpear, desaparecer)
    while (col < COLUMNAS_JUEGO && es_planta(jardin[f][col])) {
        col++;
    }
    if (col >= COLUMNAS_JUEGO) {  //se nos va de la columna
        return;
    }
    if (es_zombie(jardin[f][col])) {  //si choca con un zombie le quita vida
        quitar_vida(f, col);
    } else if (jardin[f][col] == PASTO) {  //Si la casilla era pasto, pasa a ser guisante
        jardin[f][col] = GUISANTE;
    }
}

static void mover_guisantes(void) { //con esto movemos el guisante pero en la matriz
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = COLUMNAS_JUEGO - 1; c >= 1; c--) {
            if (jardin[f][c] == GUISANTE) {
                dejar_pasto(f, c);
                llegar_guisante(f, c + 1);
            }
        }
    }
}

static void disparar_plantas(void) {   //Si estamos en turno de disparo el lanzaguisante dispara
    if (turno % config.CADA_DISPARO != 0) {  
        return;
    }
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 1; c < COLUMNAS_JUEGO; c++) { //recorremos la matriz, si es lanzaguisante y hay zombie dispara
            if (jardin[f][c] == LANZAGUISANTES && hay_zombie_a_la_derecha(f, c)) {
                llegar_guisante(f, c + 1); 
            }
        }
    }
}


static void mover_zombies(void) {  //para que zombie avanze cada X turnos
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 1; c < COLUMNAS_JUEGO; c++) {
            int tipo = jardin[f][c];
            if (!es_zombie(tipo)) { //si no es zombie, sigue
                continue;
            }
            int cada = config.CADA_PASO_ZOMBIE;  //buscamos cada cuanto el zombie da paso
            if (tipo == FLAG_ZOMBIE) {
                cada = config.CADA_PASO_FLAG; //si es flag buscamos el tiempo de flag
            }
            if (turno % cada != 0) { //si no estamos en turno de paso, no camina
                continue;
            }
            int delante = jardin[f][c - 1];  //si adelante tiene algun prop le quita vida
            if (delante == CASA) {
                vida_casa--;
                dejar_pasto(f, c);
            } else if (es_planta(delante)) {
                quitar_vida(f, c - 1);
            } else if (delante == PASTO) { //si es pasto, avanza, avanza su vida y deja el pasto
                jardin[f][c - 1] = tipo;
                vida[f][c - 1] = vida[f][c];
                dejar_pasto(f, c);
            }
        }
    }
}


static int aparecer_zombie(void) {  //aparece zombie
    int f = rand() % FILAS_JUEGO;	//fila donde aparecera
    int ultima = COLUMNAS_JUEGO - 1;  //para q aparezca en la derecha
    if (jardin[f][ultima] != PASTO) {  //si no es pasto no aparece
        return 0;
    }
    if (bandera_por_salir) {  //si estamos esperando un flag zombie
        jardin[f][ultima] = FLAG_ZOMBIE;
        bandera_por_salir = 0;
    } else {			//si spawnea solo un zombie normal
        jardin[f][ultima] = ZOMBIE;
    }
    vida[f][ultima] = config.VIDA_ZOMBIE; //ponemos la vida
    return 1;
}

static void empezar_oleada(void) {  //si empieza la oleada
    oleada++;
    fase = EN_OLEADA;
    zombies_por_salir = 3 * oleada + 2;  //cantidad de zombies q aparecen x oleada
    bandera_por_salir = 0;

    if (oleada % 2 == 0 || oleada == config.TOTAL_OLEADAS) { //si es oleada par o es la última aparece un flag
        zombies_por_salir += 3;
        bandera_por_salir = 1;
    }
}

static void controlar_oleadas(void) {
    if (fase == EN_PREPARACION || fase == EN_DESCANSO) { //si estamos en prep o descanso
        espera--;  //reducimos la espera de oleada
        if (espera <= 0) {
            empezar_oleada();
        }
    } else if (fase == EN_OLEADA) {
        if (zombies_por_salir > 0) {  //si nos quedan zombies por salir, aparecemos zombie
            if (turno % config.CADA_ZOMBIE_NUEVO == 0 && aparecer_zombie()) {
                zombies_por_salir--;
            }
        } else if (contar_zombies() == 0) { //si no hay zombies, y no quedan oleadas, ganamos
            if (oleada >= config.TOTAL_OLEADAS) {
                estado = VICTORIA;
            } else {  //Sino empezamos el descanso
                fase = EN_DESCANSO;
                espera = config.TURNOS_DESCANSO;
            }
        }
    }
}


void actualizar_juego(void) { //función con la que se actualiza el juego
    if (estado != JUGANDO) {
        return;
    }
    turno++;  //añadimos un turno
    generar_soles(); //generamos soles
    mover_guisantes(); //si hay lanzaguisantes disparando, movemos guisante
    disparar_plantas(); //si un guisante debe disparar lo hace
    mover_zombies(); //movemos zombies
    if (vida_casa <= 0) {  //si nos quedamos sin vida en la casa, perdemos
        vida_casa = 0;
        estado = DERROTA;
        return;
    }
    controlar_oleadas();
}


int obtener_celda(int fila, int col) {
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return -1;
    }
    return jardin[fila][col];
}

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
    return config.TOTAL_OLEADAS;
}
