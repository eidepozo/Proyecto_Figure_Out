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

personaje::personaje(char* filename):malla(filename){
	vida = 3;
  puntaje = 0;
}

/*void personaje::moverse(){
}

void personaje::morir(){
}*/

//gets
int personaje::getvida(){
    return this->vida;
}

int personaje::getpuntaje(){
    return this->puntaje;
}

//sets
void personaje::setvida(int vida){
	this->vida = vida;
}

void personaje::setpuntaje(int puntaje){
	this->puntaje = puntaje;
}
