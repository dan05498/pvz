#include <stdio.h>
#include "../include/entradas.h"
#include "../include/logica.h"
#include "../include/visuales.h"
#include "../include/audio.h"

typedef enum {
    MENU,
    INTRO,
    PLAY,
    PAUSE,
    DEFEAT,
    VICTORY
} estado;

int main(){
    struct configuracion cfg;

    //funciones:
    cargar_configuracion(&cfg, "config/config.txt");
    iniciar_logica(&cfg);
    // cargar_mapa(mapa, &filas, &columnas, "../config/mapa.txt");
    // cargar_base(base, &filas2, &columnas2, "../config/base.txt");

    if(!audio_iniciar()) return 1;

    //Arranque de Visuales:
    if(!inicializarVisuales()){
        return 1;
    }

    float pos_camara_x = 500.0f;

    //Ciclo de juego:
    bool jugando = true;
    estado estadoJuego = INTRO;

    int planta_elegida = GIRASOL;
    Uint64 ultimo_turno = 0;
    Uint64 ahora = 0;
    Uint64 ms_por_turno = 1000;

    mostrarNotificacion("Presiona P para guardar la partida.", 2000);    

    while(jugando){

        //Eventos (Entradas)
        SDL_Event evento;
        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_EVENT_QUIT){  //Botón de cerrar
                jugando = false;
            }

            //para ponerle pausa: 
            if(evento.type == SDL_EVENT_KEY_DOWN){ // hacemos el evento
                if(evento.key.key == SDLK_ESCAPE){ // asignamos la tecla escape
                    if(estadoJuego == PLAY){       // sie el estado de juego esta en play le ponemos pausa
                        estadoJuego = PAUSE;
                        audio_pausar(true); //aprovechando pausamos la musica
                        audio_zombies(false); // para el sonido de los zombies
                        audio_zombies_comiendo(false); // paramos el sonido si los zombies estaban comiendo
                    } else if(estadoJuego == PAUSE){// si el estado de juego esta en pausa le ponemos play 
                        estadoJuego = PLAY;
                        audio_pausar(false);//reanudamos la musica
                        audio_zombies(hay_zombie()); 
                    }
                }
            }

            if(evento.type == SDL_EVENT_KEY_DOWN){ // hacemos el evento
                if(evento.key.key == SDLK_P){
                    FILE *archivo_guardado = fopen("config/partida_guardada.txt", "w"); //hacemos el archivo de guardado con w que es write para escribir los datos que llevamos

                    fprintf(archivo_guardado, "%d %d %d\n", obtener_soles(), obtener_vida_casa(), obtener_oleada());

                    for(int i=0; i<FILAS_JUEGO; i++){
                        for(int j=0; j<COLUMNAS_JUEGO; j++){
                            fprintf(archivo_guardado, "%d ", obtener_celda(i,j));
                        }
                        fprintf(archivo_guardado, "\n");
                    }
                    fclose(archivo_guardado);
                    printf("se guardo partida con el nombre partida_guardada.txt \n");
                    mostrarNotificacion("Partida guardada correctamente.", 2000);
                }
            }      
            
            if(evento.type == SDL_EVENT_KEY_DOWN){ // hacemos el evento
                if(evento.key.key == SDLK_L){//L para cargar la partida
                    cargar_partida();
                    mostrarNotificacion("Partida cargada correctamente.", 2000);
                    estadoJuego = PLAY;
                }
            }

            if(estadoJuego == PLAY && evento.type == SDL_EVENT_KEY_DOWN){
                if(evento.key.key == SDLK_1){
                    planta_elegida = GIRASOL;
                }
                if(evento.key.key == SDLK_2){
                    planta_elegida = LANZAGUISANTES;
                }
                if(evento.key.key == SDLK_3){
                    planta_elegida = NUEZ;
                }
                if(evento.key.key == SDLK_4){
                    planta_elegida = PALA;
                }
            }

            if(estadoJuego == PLAY && evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN && evento.button.button == SDL_BUTTON_LEFT){
                if(evento.button.x >= MARGEN_X && evento.button.y >= MARGEN_Y){
                    int col = (int)((evento.button.x - MARGEN_X) / ANCHO_CELDA);
                    int fila = (int)((evento.button.y - MARGEN_Y) / ALTO_CELDA);
                    if(planta_elegida == PALA){
                        quitar_planta(fila, col);
                        planta_elegida = GIRASOL;
                    } else {
                    plantar(planta_elegida, fila, col);
                    }
                }
            }
        }

        switch (estadoJuego) {
            case MENU:
                
                break;
            case INTRO:
                if (!animacionInicio(&pos_camara_x)){
                    estadoJuego = PLAY;
                    ultimo_turno = SDL_GetTicks();
                    mostrarNotificacion("Presiona L para cargar la partida guardada.", 2000);    
                }
                audio_zombies(false);
                break;
            case PLAY:
                ahora = SDL_GetTicks();
                if(ahora - ultimo_turno >= ms_por_turno){
                    actualizar_juego();
                    ultimo_turno = ahora;
                }

                if(estado_juego() == VICTORIA){
                    printf("Ganaste\n");    //por ahora donde no hay pantalla de victoria xD
                    estadoJuego = VICTORY;
                    audio_reproducir(SFX_Victoria); // audio de la victoria
                    audio_pausar(true);
                    audio_zombies(false);
                    audio_zombies_comiendo(false);
                }
                if(estado_juego() == DERROTA){
                    printf("Perdiste\n");
                    estadoJuego = DEFEAT;
                    audio_reproducir(SFX_Derrota); // audio de la derrota 
                    audio_pausar(true);
                    audio_zombies(false);
                    audio_zombies_comiendo(false);
                }
                audio_zombies(hay_zombie());//osea mientras este el juego activo en PLay usamos el audio si hay zombies

                audio_zombies_comiendo(hay_zombie_comiendo()); // se parentesis de dentro es para usar el bool igual que hay zombie
                break;
            case PAUSE:
                break;
            case VICTORY:
                break;
            case DEFEAT:
                break;
        }

        loopVisuales(pos_camara_x, estadoJuego, planta_elegida);
    }

    cerrarSDL();

    return 0;
}
