#ifndef ENTRADAS_H
#define ENTRADAS_H

//configuracion
    struct configuracion{
        int VIDA_CASA_INICIAL; 
        int VIDA_ZOMBIE;
        int VIDA_NUEZ;
        int VIDA_GIRASOL;
        int VIDA_LANZAGUISANTES;
        int SOLES_INICIALES;
        int COSTO_LANZAGUISANTES;
        int COSTO_GIRASOL;
        int COSTO_NUEZ;
        int SOLES_AUTOMATICO;
        int SOLES_GIRASOL;
        int CADA_SOL_AUTOMATICO;
        int CADA_SOL_GIRASOL;
        int CADA_DISPARO;
        int CADA_PASO_ZOMBIE;
        int CADA_PASO_FLAG;
        int CADA_ZOMBIE_NUEVO;
        int TURNOS_PREPARACION;
        int TURNOS_DESCANSO;
        int FASE_PREPARACION;
        int FASE_OLEADA;
        int FASE_DESCANSO;
        int VIDA_LANZAHIELO;
        int COSTO_LANZAHIELO;
        int CADA_DISPARO_HIELO;
        int TURNOS_CONGELADO;
        int VIDA_GIGANTE;
        int DANO_GIGANTE;
        int CADA_PASO_GIGANTE;
    };//sin este puntocoma el main da error

    void cargar_configuracion(struct configuracion *cfg, const char *ruta);
    void cargar_mapa(int matriz[8][13], int *filas, int *columnas, const char *ruta);
    void cargar_base(int base[5][11], int *filas2, int *columnas2, const char *ruta1);

    #endif
