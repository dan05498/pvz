#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../include/audio.h" 

static MIX_Mixer *mixer = NULL;
static MIX_Track *track_musica = NULL;
static MIX_Track *track_comer = NULL;
static MIX_Audio *efectos[SFX_Total];
static bool comiendo_activo = false;

#define RUTA_MUSICA"assets/pvz_soundtrack1.mp3"

static void reproducir_en_bucle(MIX_Track *t){
    SDL_PropertiesID opciones = SDL_CreateProperties();
    SDL_SetNumberProperty(opciones, MIX_PROP_PLAY_LOOPS_NUMBER, -1); //para infinito el -1
    MIX_PlayTrack(t,opciones);
    SDL_DestroyProperties(opciones);//MIX_PlayTrack hace una copia y la guarda
}

bool audio_iniciar(void){
    if(!SDL_Init(SDL_INIT_AUDIO)){
        SDL_Log("Error SDL_Init: %s", SDL_GetError());
        return false;
    }
    if(!MIX_Init()){
        SDL_Log("Error MIX_Init: %s ", SDL_GetError());
        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if(!mixer){
        SDL_Log("Error creando mixer;%s", SDL_GetError());
        return false;
    }

    MIX_Audio *musica = MIX_LoadAudio(mixer, RUTA_MUSICA, false);
    if(!musica){
        SDL_Log("Error cargando musica: %s", SDL_GetError());
        return false;
    }
    track_musica = MIX_CreateTrack(mixer);
    MIX_SetTrackAudio(track_musica, musica);
    reproducir_en_bucle(track_musica);

    return true;
}

    void audio_reporducir(Efecto e){
        if(e < 0 || e>=SFX_Total)return;
        MIX_PlayAudio(mixer, efectos[e]);
    }
