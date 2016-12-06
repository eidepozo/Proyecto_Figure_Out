#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include "maths_funcs.h"
#include "tools.h"
#include "malla.h"
//#include "stb_image.h"

using namespace std;
malla::malla(char* filename){ //constructor que se llama cuando se crea un nuevo objeto de la clase
	vec3 pos = vec3(0,0,0);//malla
	this->filename = filename;
	assert(load_mesh(filename, &vao, &numvertices));
}

malla::malla(){
    vec3 pos = vec3(0,0,0);
}

// gets
GLuint malla::getvao(){
    return this->vao;
}

int malla::getnumvertices(){
    return this->numvertices;
}

vec3 malla::getpos(){
    return this->getpos();
}

char* malla::getfilename(){
    return this->filename;
}

//sets
void malla::setvao(GLuint vao){
	this->vao = vao;
}

void malla::setnumvertices(int nvertices){
    this->numvertices = nvertices;
}

void malla::setpos(vec3 p){
    this->pos = p;
}

void malla::setfilename(char *filename){
	this->filename = filename;
}


