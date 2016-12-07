/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| This demo uses the Assimp library to load a mesh from a file, and supports   |
| many formats. The library is VERY big and complex. It's much easier to write |
| a simple Wavefront .obj loader. I have code for this in other demos. However,|
| Assimp will load animated meshes, which will we need to use later, so this   |
| demo is a starting point before doing skinning animation                     |
\******************************************************************************/
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "maths_funcs.h"
#include "gl_utils.h"
#include "tools.h"
#include "piso.h"
#include "personaje.h"
#include "obstaculo.h"
#include "moneda.h"
#include "corazon.h"
#include "malla.h"
#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"
#define num_obstaculos 3 //macros
#define num_monedas 1
#define num_corazones 1
#define num_personajes 1
#define num_piso 1

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 1280;
int g_gl_height = 690;
GLFWwindow* g_window = NULL;

/* Definicion previa de la ubicacion de los objetos */
vec3 pos_obstaculos[] = {
	vec3 (1.5, -0.15, 2.5),
	vec3 (-2.0, -0.15, 3.75),
	vec3 (-1.25, -0.15, 3.75)
};

vec3 pos_monedas[] = {
	vec3(-1.65,-0.15,3.75)
};

vec3 pos_piso[] = {
	vec3(0.0,-0.35,-0.15)
};

vec3 pos_corazones[] = {
	vec3(2.0,0.2,5.0)
};

vec3 pos_personaje[] = {
	vec3(0.0,-0.15,5.75)
};

int main(){
	init(g_gl_width, g_gl_height); /* Define elementos basicos de OpenGL, se ubica en tools.cpp */

	/* Creacion de Objetos */
	personaje *p1 = new personaje((char*)"mallas/personaje.obj");
	obstaculo *o [num_obstaculos]; /* Arreglo de objetos obstaculo */
	for (int i = 0; i < num_obstaculos; i++){
		o[i] = new obstaculo((char*)"mallas/obstaculo.obj");
	}
	moneda *m1 = new moneda((char*)"mallas/moneda.obj");
  corazon *c1 = new corazon((char*)"mallas/corazon.obj");
  piso *f1 = new piso((char*)"mallas/piso.obj");

//-------------------------------CREATE SHADERS-------------------------------
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	int obj_mat_location = glGetUniformLocation(shader_programme,"obj");

	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
	// input variables
	float near = 0.1f; // clipping plane
	float far = 100.0f; // clipping plane
	float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
	float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
	// matrix components
	float range = tan (fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	GLfloat proj_mat[] = {
		Sx, 0.0f, 0.0f, 0.0f,
		0.0f, Sy, 0.0f, 0.0f,
		0.0f, 0.0f, Sz, -1.0f,
		0.0f, 0.0f, Pz, 0.0f
	};

	float cam_speed = 1.0f; //1.0 unites per second
	float cam_yaw_speed = 30.0f; // 30 degrees per second
	float cam_pos[] = {0.0f, 0.1f, 7.5f}; // orig 0.0, 0.0, 5.0 - don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	/* Variables auxiliares que permiten salto, inicio y pausa */
	bool start = false;
	bool jump = false;
	float velocityY = 0.0f;
	float gravity = 0.005f;

/*---------------------------SET RENDERING DEFAULTS---------------------------*/
	glUseProgram (shader_programme); // Installs a program object as part of current rendering state
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
	//la instruccion de arriba nos permite pasar matrices al shader

	/* Ajustes en las matrices de cada objeto, cambio de ubicacion, scale, etc. */
	p1->M = translate (identity_mat4(), pos_personaje[0]);
	for(int i = 0; i < num_obstaculos; i++){
		o[i]->M = translate(identity_mat4(), pos_obstaculos[i]);
	}
	m1->M = translate (identity_mat4(), pos_monedas[0]);
	c1->M = translate (identity_mat4(), pos_corazones[0]);
	f1->M = scale (f1->M, vec3(5.0,0.0,15.0));
	f1->M = translate (identity_mat4(), pos_piso[0]);

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);

		//glUseProgram (shader_programme);

		//personaje, piso, obstaculos, moneda, corazon
		glUniformMatrix4fv (obj_mat_location, 1 ,GL_FALSE, p1->M.m);
    glBindVertexArray(p1->getvao());
    glDrawArrays(GL_TRIANGLES,0,p1->getnumvertices());

		for(int i = 0; i < num_obstaculos; i++){
			glUniformMatrix4fv (obj_mat_location, 1 ,GL_FALSE, o[i]->M.m);
      glBindVertexArray(o[i]->getvao());
      glDrawArrays(GL_TRIANGLES,0,o[i]->getnumvertices());
		}

		glUniformMatrix4fv (obj_mat_location, 1 ,GL_FALSE, m1->M.m);
    glBindVertexArray(m1->getvao());
    glDrawArrays(GL_TRIANGLES,0,m1->getnumvertices());

		glUniformMatrix4fv (obj_mat_location, 1 ,GL_FALSE, c1->M.m);
    glBindVertexArray(c1->getvao());
    glDrawArrays(GL_TRIANGLES,0,c1->getnumvertices());

    glUniformMatrix4fv (obj_mat_location, 1 ,GL_FALSE, f1->M.m);
   	glBindVertexArray(f1->getvao());
    glDrawArrays(GL_TRIANGLES,0,f1->getnumvertices());

		// update other events like input handling
		glfwPollEvents ();

		//llamada a funcion gameplay que controla a la camara2, se ubica en tools.cpp
	  bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed);

		/* Interaccion teclado con la camara1 */
		if (glfwGetKey (g_window, GLFW_KEY_A)){
			if(start){
				cam_pos[0] -= 3*cam_speed * elapsed_seconds;
				p1->M.m[12] -= 0.05f;
				if (p1->M.m[12]< -2.30f && p1->M.m[12]> -2.40f){ //portal A <-
					cam_pos[0] = 2.10f;
					p1->M.m[12] = 2.10f;
				}
				cam_moved = true;
			}
		}

		if (glfwGetKey (g_window, GLFW_KEY_D)){
			if(start){
				cam_pos[0] += 3*cam_speed * elapsed_seconds;
				p1->M.m[12] += 0.05f;
				if (p1->M.m[12]> 2.30f && p1->M.m[12]<2.40f){//portal B ->
					cam_pos[0] = -2.10f;
					p1->M.m[12] = -2.10f;
				}
				cam_moved = true;
			}
		}

		//pausa
		if (glfwGetKey (g_window, GLFW_KEY_P)) {
			if(start) start = false;

			else if((!start) && cam_pos[2]<6.0f && cam_pos[2]>-5.0f) start = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_F)){//update-camera (focus)
			cam_pos[0] = 0.0f;
			cam_pos[1] = 0.1f;
			//cam_pos[2] = 7.5f;
			cam_yaw = 0.0f;
			//p1->M = translate (identity_mat4(), pos_personaje[0]);
			cam_moved = true;
		}

		//inicio
		if (glfwGetKey (g_window, GLFW_KEY_ENTER)) {
			if(!start) start = true;
		}

		if (start){
			cam_pos[2] -= 3.0*cam_speed * elapsed_seconds;
			p1->M.m[14] -= 0.05f;
			if (cam_pos[2]<-5.0f){ // largo
				cam_pos[0] = 0.0f;//reseteo de la camara
				cam_pos[1] = 0.1f;
				cam_pos[2] = 7.5f;
				p1->M = translate (identity_mat4(), pos_personaje[0]);
				start = false;
			}
			cam_moved = true;
		}

		//salto
		if (glfwGetKey (g_window, GLFW_KEY_SPACE)) {
			if(start && !jump){
				jump = true;
				velocityY = 0.1f; //orig 0.12f
			}
		}
		if (jump){
			velocityY -= gravity; //aumentamos la posicion con un valor que va disminuyendo
			cam_pos[1] += velocityY;
			p1->M.m[13] += velocityY;
			if (cam_pos[1]<0.0f){
				cam_pos[1] = 0.1;
				p1->M.m[13] = -0.15;
				velocityY = 0.0;
				jump = false;
			}
			cam_moved = true;
		}

		//printf("cam_pos2 = %f\n", cam_pos[2]);
		//printf("M.m 13 eje y = %f\n", p1->M.m[13]);*/
		// update view matrix
		if (cam_moved) {
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw); //
			mat4 view_mat = R * T;
			glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
		}

		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
