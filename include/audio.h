#ifndef Audio_H
#define Audio_H

#include <stdbool.h>

typedef enum{ // se puede ver en la clase de hoy lunes 9/11
    SFX_Plantar,
    SFX_Sol,
    SFX_Guizante,
    SFX_Derrota,
    SFX_Victoria,
    SFX_Total,
} efecto; //son sonidos que no se repiten en bucle los bool suelen ser para bucles

bool audio_iniciar(void);   //musica en bucle
void audio_reproducir(efecto e);    //superponer
void audio_zombies_comiendo(bool zombie_comiendo); //con los zombies atacando
void audio_derrota(void); //cuando perdemos
void audio_victoria(void);
void audio_pausar(bool pausado); // para pausar y poner play usando bool si esta pausado es true si no es false
void audio_zombies(bool zombie); // boleano para reproducir el zonido si hay zombie

#endif
