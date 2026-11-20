#ifndef VISUALES_H
#define VISUALES_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define NOMBRE_VENTANA  "pvz"

#define ANCHO                   1280.0f
#define ALTO                    720.0f

#define FILAS                   5
#define COLUMNAS                11

#define MARGEN_X                -52.0f      //80.0f - ANCHO_CELDA (127.0f)
#define MARGEN_Y                103.0f
#define ANCHO_CELDA             127.0f
#define ALTO_CELDA              116.0f

#define MARGEN_X_SELECTOR       20.0f
#define MARGEN_Y_SELECTOR       0.0f

#define POS_CAMARA_JUGANDO      300.0f
#define VEL_CAMARA              6.0f   

#define VEL_ZOMBI_GUISANTE      25.0f

// Funciones de inicio y cierre
bool inicializarVisuales(void);
void cerrarSDL(void);

// Funciones para cargar y dibujar objetos
SDL_Texture *cargarTextura(const char *archivo);
void prepararFondo(void); // Pinta de gris la pantalla antes de dibujar
void RenderizarFondo(SDL_Renderer*, SDL_Texture*, float);

//Prueba de la cuadrícula
void RenderizarCuadriculaDebug(SDL_Renderer* renderer);

void loopVisuales(float, int, int);

bool animacionInicio(float*);

void imprimirTextura(int, SDL_FRect, int, int);
void fondoPausa();
void pantallaDerrota();
void dibujarUI(int);
void dibujarSoles();

void mostrarNotificacion(char*, Uint64);
void renderizarNotificacion();
#endif