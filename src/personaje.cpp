#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include "maths_funcs.h"
#include "tools.h"
#include "personaje.h"
using namespace std;

personaje::personaje(const char* filename, GLuint shader_program, GLuint face_type):malla(filename, shader_program, face_type){
	vida = 3;
  puntaje = 0;
	nsaltos = 3;
}


//gets
int personaje::getvida(){
    return this->vida;
}

int personaje::getpuntaje(){
    return this->puntaje;
}

int personaje::getnsaltos(){
    return this->nsaltos;
}
//sets
void personaje::setvida(int vida){
	this->vida = vida;
}

void personaje::setpuntaje(int puntaje){
	this->puntaje = puntaje;
}

void personaje::setnsaltos(int nsaltos){
	this->nsaltos = nsaltos;
}

void personaje::estadoActual(){
	printf("Puntaje: %i , Vidas: %i, Saltos: %i\n", puntaje, vida, nsaltos);
}
