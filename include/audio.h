#ifndef Audio_H
#define Audio_H

#include <stdbool.h>

typedef enum{ // se puede ver en la clase de hoy lunes 9/11
    SFX_Plantar,
    SFX_Sol,
    SFX_Guizante,//porque la coma
    SFX_Derrota,
    SFX_Total,
} Efecto;

bool audio_iniciar(void);   //musica en bucle
void audio_reproducir(Efecto e);    //superponer
void audio_zombies_comiendo(bool comiendo); //con los zombies atacando
void audio_derrota(void); //cuando perdemos
void audio_cerrar(void); // cuando termine el codigo

#endif
