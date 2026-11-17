#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../include/audio.h" 
#include <logica.h> // para icluir los sonidos de los zombies

static MIX_Mixer *mixer = NULL;
static MIX_Track *track_musica = NULL;
static MIX_Track *track_comer = NULL;
static MIX_Audio *efectos[SFX_Total];
static MIX_Track *track_zombie = NULL;
static bool zombies_activo = false;
static bool comiendo_activo = false;

#define RUTA_MUSICA     "assets/pvz_soundtrack1.mp3" //asignamos la cancion
#define RUTA_SONIDO_ZOMBIES "assets/pvz_zombTrack.mp3" //el sonido de los zombies 


static void reproducir_en_bucle(MIX_Track *t){
    SDL_PropertiesID opciones = SDL_CreateProperties();
    SDL_SetNumberProperty(opciones, MIX_PROP_PLAY_LOOPS_NUMBER, -1); //para infinito el -1
    MIX_PlayTrack(t,opciones);
    SDL_DestroyProperties(opciones);//MIX_PlayTrack hace una copia y la guarda por eso podemos destruir
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

    //sonido zombies
    MIX_Audio *sonido_zombies = MIX_LoadAudio(mixer, RUTA_SONIDO_ZOMBIES, false);
    track_zombie = MIX_CreateTrack(mixer);
    MIX_SetTrackAudio(track_zombie, sonido_zombies);

    return true;
}

    void audio_reporducir(Efecto e){
        if(e < 0 || e>=SFX_Total)return;
        MIX_PlayAudio(mixer, efectos[e]);
    }

    void audio_pausar(bool pausado){
        if(pausado){
            MIX_PauseTrack(track_musica); // para pausar
            MIX_PauseTrack(track_zombie);
        }else{
            MIX_ResumeTrack(track_musica); // de lo contrario sigue 
            MIX_ResumeTrack(track_zombie);
        }
    }

    void audio_zombies(bool hay_zombie){
        if(hay_zombie && !zombies_activo){ // osea mientras hay zombies y el zombie activo que era false ahora se niegue osea sea verdadero
            reproducir_en_bucle(track_zombie);
            zombies_activo = true;
        } else if(!hay_zombie && zombies_activo){ // lo contrario de lo de arriba
            MIX_StopTrack(track_zombie, 0);
            zombies_activo = false;
        }
    }