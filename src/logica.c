#include <stdio.h> //para poder usar el FILE * etc etc cargar partida en resumen
#include <stdlib.h>
#include <time.h>
#include "../include/logica.h"
#include "../include/audio.h" // para los sfx osea los efectos de sonido

#define EN_PREPARACION   0
#define EN_OLEADA        1
#define EN_DESCANSO      2

static struct configuracion config;

static int jardin[FILAS_JUEGO][COLUMNAS_JUEGO];  //Matriz de Juego
static int vida[FILAS_JUEGO][COLUMNAS_JUEGO];   //Matriz con la vida de las cosas del jardín
static int guisantes[FILAS_JUEGO][COLUMNAS_JUEGO];
static int hielos[FILAS_JUEGO][COLUMNAS_JUEGO];
static int congelado[FILAS_JUEGO][COLUMNAS_JUEGO];
static int puntaje;
static int record;
static int gigantes_por_salir;
static int soles_caidos;

static int estado;   //jugando, victoria, derrota
static int fase;	//preparacion, oleada, descanso
static int turno;	//turno de la partida
static int soles;
static int vida_casa;
static int oleada;
static int espera;
static int zombies_por_salir;
static int bandera_por_salir;
static int espera_plantar;


static int es_planta(int codigo) {  //si es planta el zombie o el guisante toma una decisión
    return codigo == GIRASOL || codigo == LANZAGUISANTES || codigo == NUEZ || codigo == LANZAHIELO;
}

static int es_zombie(int codigo) { //Si es zombie el zombie o guisante toma una decisión
    return codigo == ZOMBIE || codigo == FLAG_ZOMBIE || codigo== GIGANTE;
}

static int dano_zombie(int tipo) {
    if (tipo == GIGANTE) {
        return config.DANO_GIGANTE;
    }
    return 1;
}

static int puntos_zombie(int tipo) {
    if (tipo == GIGANTE) {
        return 50;
    }
    if (tipo == FLAG_ZOMBIE) {
        return 15;
    }
    return 10;
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
    if (codigo== GIGANTE) {
        return config.VIDA_GIGANTE + oleada/2;
    }
    if (es_zombie(codigo)) {
        return config.VIDA_ZOMBIE + oleada/2;
    }
    return 0;
}

static void dejar_pasto(int f, int c) {  //Cada que el zombie o guisante se mueve devolvemos pasto
    jardin[f][c] = PASTO;
    vida[f][c] = 0;
    congelado[f][c]=0;
}

static void quitar_vida(int f, int c, int cantidad) {   //quitamos la vida en la matriz de vida
    vida[f][c]-=cantidad;
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

bool hay_zombie(void){ // validar si hay zombies
    return contar_zombies() > 0; // ese cero va en relacion con lo del audio 
}

static int hay_zombie_a_la_derecha(int f, int c) {  //sabemos si el lanzaguisante dispara
    for (int k = c + 1; k < COLUMNAS_JUEGO; k++) {
        if (es_zombie(jardin[f][k])) {
            return 1;
        }
    }
    return 0;
}

static void cargar_record(void) {
    record = 0;
    FILE *archivo = fopen("config/record.txt", "r");
    if (archivo == NULL) {
        return;
    }
    fscanf(archivo, "%d", &record);
    fclose(archivo);
}

void guardar_record(void) {
    if (puntaje <= record) {
        return;
    }
    record = puntaje;
    FILE *archivo = fopen("config/record.txt", "w");
    if (archivo == NULL) {
        return;
    }
    fprintf(archivo, "%d\n", record);
    fclose(archivo);
}

void iniciar_logica(struct configuracion *cfg) { //Iniciamos la matriz de juego y vida
    config = *cfg;
    
    FILE *archivo_mapa = fopen("config/mapa.txt", "r");
    if (archivo_mapa != NULL) {
        for (int f = 0; f < FILAS_JUEGO; f++) {
            for (int c = 0; c < COLUMNAS_JUEGO; c++) {
                fscanf(archivo_mapa, "%d", &jardin[f][c]);
                vida[f][c] = 0;
                guisantes[f][c] = 0;
                hielos[f][c] = 0;
                congelado[f][c] = 0;
            }
        }
        fclose(archivo_mapa);
    } else {
        // Respaldo por si no encuentra el archivo manera manual:
        for (int f = 0; f < FILAS_JUEGO; f++) {
            jardin[f][0] = CASA;
            vida[f][0] = 0;
            guisantes[f][0] = 0;
            for (int c = 1; c < COLUMNAS_JUEGO; c++) {
                jardin[f][c] = PASTO;
                vida[f][c] = 0;
                guisantes[f][c] = 0;
                hielos[f][c] = 0;
                congelado[f][c] = 0;
            }
        }
    }

    soles_caidos=0;
    puntaje = 0;
    espera_plantar=0;
    gigantes_por_salir = 0;
    cargar_record();
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
    if (tipo == LANZAHIELO) {
        return config.COSTO_LANZAHIELO;
    }
    return 0;
}

int plantar(int tipo, int fila, int col) { //Función para plantar
    if (estado != JUGANDO || !es_planta(tipo)) {  //Restricciones, no tenemos soles, fuera de la matriz
        return 0;		audio_reproducir(SFX_SinSOLES);					     //estamos intentando plantar donde hay otra planta, etc
        
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
    if(espera_plantar>0){ audio_reproducir(SFX_SinSOLES);
        return 0;
    }    
    if (soles < costo_planta(tipo)) {audio_reproducir(SFX_SinSOLES);
        return 0;
        
    }
    soles -= costo_planta(tipo);   //nos restamos el costo de la planta 
    espera_plantar = config.ESPERA_PLANTAR;
    jardin[fila][col] = tipo;audio_reproducir(SFX_Plantar);		//ponemos la planta en la matriz
    vida[fila][col] = vida_inicial(tipo);	//ponemos la vida de la planta
    return 1;
}

int quitar_planta(int fila, int col) {
    if (estado != JUGANDO) {
        return 0;
    }
    if (fila < 0 || fila >= FILAS_JUEGO) {
        return 0;
    }
    if (col < 1 || col > COLUMNAS_JUEGO - 2) {
        return 0;
    }
    if (!es_planta(jardin[fila][col])) {
        return 0;
    }
    dejar_pasto(fila, col);
    return 1;
}

static void generar_soles(void) {
    if (turno % config.CADA_SOL_AUTOMATICO == 0) {   //se genera sol autom cada X turnos
        soles += config.SOLES_AUTOMATICO;
        soles_caidos++;
        audio_reproducir(SFX_Sol);
    }
    if (turno % config.CADA_SOL_GIRASOL == 0) {   //se genera sol de girasol cada X turnos
        for (int f = 0; f < FILAS_JUEGO; f++) {
            for (int c = 1; c < COLUMNAS_JUEGO; c++) {
                if (jardin[f][c] == GIRASOL) {
                    soles += config.SOLES_GIRASOL;
                    audio_reproducir(SFX_Sol);
                }
            }
        }
    }
}


static void golpear_zombie(int f, int c, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (es_zombie(jardin[f][c])) {
            int tipo = jardin[f][c];
            quitar_vida(f, c, 1);
            if (jardin[f][c] == PASTO){audio_reproducir(SFX_zombieDD); //audio para cuando muera el zombie
                puntaje +=puntos_zombie(tipo);
            }
        }
    }
}

static void soltar_guisante(int f, int c) {
    if (c >= COLUMNAS_JUEGO) {
        return;
    }
    if (es_zombie(jardin[f][c])) {
        golpear_zombie(f, c, 1);
    } else {
        guisantes[f][c]++;
    }
}

static void mover_guisantes(void) {
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = COLUMNAS_JUEGO - 1; c >= 1; c--) {
            int cantidad = guisantes[f][c];
            if (cantidad == 0) {
                continue;
            }
            guisantes[f][c] = 0;
            if (c + 1 >= COLUMNAS_JUEGO) {
                continue;
            }
            if (es_zombie(jardin[f][c + 1])) {
                golpear_zombie(f, c + 1, cantidad);
            } else {
                guisantes[f][c + 1] += cantidad;
            }
        }
    }
}

static void golpear_zombie_hielo(int f, int c, int cantidad) {
    golpear_zombie(f, c, cantidad);
    if (es_zombie(jardin[f][c])) {
        congelado[f][c] = config.TURNOS_CONGELADO;
    }
}

static void soltar_hielo(int f, int c) {
    if (c >= COLUMNAS_JUEGO) {
        return;
    }
    if (es_zombie(jardin[f][c])) {
        golpear_zombie_hielo(f, c, 1);
    } else {
        hielos[f][c]++;
    }
}

static void mover_hielos(void) {
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = COLUMNAS_JUEGO - 1; c >= 1; c--) {
            int cantidad = hielos[f][c];
            if (cantidad == 0) {
                continue;
            }
            hielos[f][c] = 0;
            if (c + 1 >= COLUMNAS_JUEGO) {
                continue;
            }
            if (es_zombie(jardin[f][c + 1])) {
                golpear_zombie_hielo(f, c + 1, cantidad);
            } else {
                hielos[f][c + 1] += cantidad;
            }
        }
    }
}

static void resolver_choques(void) {
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 1; c < COLUMNAS_JUEGO; c++) {
            if (guisantes[f][c] > 0 && es_zombie(jardin[f][c])) {
                golpear_zombie(f, c, guisantes[f][c]);
                guisantes[f][c] = 0;
            }
            if(hielos[f][c]>0 && es_zombie(jardin[f][c])){
                golpear_zombie_hielo(f,c, hielos[f][c]);
                hielos[f][c]=0;
            }
        }
    }
}


static void disparar_plantas(void) {
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 1; c < COLUMNAS_JUEGO; c++) {
            if (jardin[f][c] == LANZAGUISANTES && turno % config.CADA_DISPARO == 0 && hay_zombie_a_la_derecha(f, c)) {
                soltar_guisante(f, c + 1);
                audio_reproducir(SFX_Guizante);
            }
            if (jardin[f][c] == LANZAHIELO && turno % config.CADA_DISPARO_HIELO == 0 && hay_zombie_a_la_derecha(f, c)) {
                soltar_hielo(f, c + 1);
                audio_reproducir(SFX_Guizante);
            }
        }
    }
}


static void mover_zombies(void) {
    for (int f = 0; f < FILAS_JUEGO; f++) {
        for (int c = 1; c < COLUMNAS_JUEGO; c++) {
            int tipo = jardin[f][c];
            if (!es_zombie(tipo)) {
                continue;
            }
            if (congelado[f][c] > 0) {
                congelado[f][c]--;
                continue;
            }
            int cada = config.CADA_PASO_ZOMBIE;
            if (tipo == FLAG_ZOMBIE) {
                cada = config.CADA_PASO_FLAG;
            }
            if (tipo == GIGANTE) {
                cada = config.CADA_PASO_GIGANTE;
            }
            if (turno % cada != 0) {
                continue;
            }
            int dano = dano_zombie(tipo);
            int delante = jardin[f][c - 1];
            if (delante == CASA) {
                vida_casa -= dano;
                audio_reproducir(SFX_AWP);
                dejar_pasto(f, c);
                audio_reproducir(SFX_JEJE);
            } else if (es_planta(delante)) {
                quitar_vida(f, c - 1, dano);
            } else if (delante == PASTO) {
                jardin[f][c - 1] = tipo;
                vida[f][c - 1] = vida[f][c];
                dejar_pasto(f, c);
            }
        }
    }
}

bool hay_zombie_comiendo(void){
    for(int f=0; f<FILAS_JUEGO ; f++){ //recorre filas
        for(int c=1; c<COLUMNAS_JUEGO; c++){ //recorre columnas
            if(es_zombie(jardin[f][c]) && congelado[f][c] && es_planta(jardin[f][c-1])){ // si la es zombie tiene un es planta al lado significa que le quitara vida por lo tanto podemos modificar el booleando para que suene el sonido de comida
                return true; // cambia el bool a true
            }
        }
    }
    return false; // lo contrario
}


static int aparecer_zombie(void) {
    int f = rand() % FILAS_JUEGO;
    int ultima = COLUMNAS_JUEGO - 1;
    if (jardin[f][ultima] != PASTO) {
        return 0;
    }
    int tipo = ZOMBIE;
    if (bandera_por_salir) {
        tipo = FLAG_ZOMBIE;
        bandera_por_salir = 0;
    } else if (gigantes_por_salir > 0 && zombies_por_salir > 0 && rand() % zombies_por_salir < gigantes_por_salir) {
        tipo = GIGANTE;
        gigantes_por_salir--;
    }
    jardin[f][ultima] = tipo;
    vida[f][ultima] = vida_inicial(tipo);
    return 1;
}

static void empezar_oleada(void) {
    audio_reproducir(SFX_Oleada);
    oleada++;
    fase = EN_OLEADA;
    zombies_por_salir = 4 + oleada / 2;
    if (zombies_por_salir > 12) {
        zombies_por_salir = 12;
    }
    bandera_por_salir = 0;
    gigantes_por_salir = 0;

    if (oleada % 2 == 0) {
        zombies_por_salir += 3;
        bandera_por_salir = 1;
    }
    if (oleada >= 4) {
        gigantes_por_salir = (oleada - 2) / 2;
        if (gigantes_por_salir > zombies_por_salir / 2) {
            gigantes_por_salir = zombies_por_salir / 2;
        }
    }
}

static void controlar_oleadas(void) {
    if (fase == EN_PREPARACION || fase == EN_DESCANSO) {
        espera--;
        if (espera <= 0) {
            empezar_oleada();
        }
    } else if (fase == EN_OLEADA) {
        if (zombies_por_salir > 0) {
            int cada_nuevo = config.CADA_ZOMBIE_NUEVO - oleada / 4;
            if (cada_nuevo < 2) {
                cada_nuevo = 2;
            }
            if (turno % cada_nuevo == 0 && aparecer_zombie()) {
                zombies_por_salir--;
            }
        } else if (contar_zombies() == 0) {
            puntaje += 50 * oleada;
            fase = EN_DESCANSO;
            espera = config.TURNOS_DESCANSO;
        }
    }
}

void actualizar_juego(void) { //función con la que se actualiza el juego
    if (estado != JUGANDO) {
        return;
    }
    turno++;  //añadimos un turno
    if(espera_plantar>0){
        espera_plantar--;
    }    
    generar_soles(); //generamos soles
    mover_guisantes(); //si hay lanzaguisantes disparando, movemos guisante
    mover_hielos();
    disparar_plantas(); //si un guisante debe disparar lo hace
    mover_zombies(); //movemos zombies
    resolver_choques();
    if (vida_casa <= 0) {  //si nos quedamos sin vida en la casa, perdemos
        vida_casa = 0;
        estado = DERROTA;
        guardar_record();
        return;
    }
    controlar_oleadas();
}


int obtener_celda(int fila, int col) {   //dejé esta función para Tarea 3 pueda dibujar y saber qué hay en cada celda
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return -1;
    }
    return jardin[fila][col];
}

//Funciones aux

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

int obtener_guisante(int fila, int col) {
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return 0;
    }
    return guisantes[fila][col];
}

int obtener_dano(int fila, int col) {
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return 0;
    }
    int maxima = vida_inicial(jardin[fila][col]);
    if (maxima <= 0) {
        return 0;
    }
    int actual = vida[fila][col];
    if (actual * 3 > maxima * 2) {
        return 0;
    }
    if (actual * 3 > maxima) {
        return 1;
    }
    return 2;
}

int obtener_soles_caidos(void) {
    return soles_caidos;
}

int obtener_hielo(int fila, int col) {
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return 0;
    }
    return hielos[fila][col];
}

int obtener_congelado(int fila, int col) {
    if (fila < 0 || fila >= FILAS_JUEGO || col < 0 || col >= COLUMNAS_JUEGO) {
        return 0;
    }
    return congelado[fila][col];
}

int obtener_puntaje(void) {
    return puntaje;
}

int obtener_record(void) {
    if (puntaje > record) {
        return puntaje;
    }
    return record;
}

void cargar_partida(void){
    FILE *cargar = fopen("config/partida_guardada.txt", "r");

    if(cargar == NULL){
        return; // para que no modifique nada si no hay nada que cargar
    }

    fscanf(cargar, "%d %d %d", &soles, &vida_casa, &oleada); // leemos y cargamos lo que teniamos

    for(int F=0; F<FILAS_JUEGO;F++){
        for(int C=0;C<COLUMNAS_JUEGO;C++){
            fscanf(cargar, "%d", &jardin[F][C]); //cargamos el jardin
        }
    }
    fclose(cargar); //ya que cargamos

}

