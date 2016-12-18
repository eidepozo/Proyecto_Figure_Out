#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "malla.h"

using namespace std;

class personaje: public malla{
    private:
        int vida;
        int puntaje;
        int nsaltos;

    public:
        personaje(const char *filename, GLuint shadprog, GLuint face_type);
        ~personaje();

        // gets
        int getvida();
        int getpuntaje();
        int getnsaltos();

        // sets
        void setvida(int vida);
        void setpuntaje(int puntaje);
        void setnsaltos(int nsaltos);
        void estadoActual();
};

#endif
