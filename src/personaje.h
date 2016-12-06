#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "malla.h"

using namespace std;

class personaje: public malla{
    private:
        int vida;
        int puntaje;
        
    public:
        personaje(char *filename);
        ~personaje();
        void moverse();
        void morir();

        // gets
        int getvida();
        int getpuntaje();

        // sets
        void setvida(int vida);
        void setpuntaje(int puntaje);        
};

#endif
