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
#include "malla.h"
#include "personaje.h"
#include <time.h>
#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define num_personajes 1
#define num_obstaculos 46 //macros
#define num_piso 3
#define num_monedas 18
#define num_corazones 2

// keep track of window size for things like the viewport and the mouse cursor

/* Definicion previa de la ubicacion de los objetos */

/*vec3 pos_obstaculos[] = {
	vec3 (0.15, 0.0, 0.0), //prueba
	vec3 (0.0, 0.0, -0.9),
	vec3 (0.9, 0.0, -0.18),
	vec3 (0.9, 0.0, -3.0), //aqui buscar como hacer que vayan vair
	vec3 (0.6, 0.0, -6.0),
	vec3 (-0.3, 0.0, -6.0),
	vec3 (0.9, 0.0, -9.0),
	vec3 (0.6, 0.0, -12.0),
	vec3 (0.0, 0.0, -15.0)
};
*/
vec3 pos_obstaculos[] = {
     //prueba
    vec3 (-1.95, 0.0, -42.0),
    vec3 (-1.65, 0.0, -42.0),
    vec3 (-1.35, 0.0, -42.0), //aqui buscar como hacer que vayan vair
    vec3 (-1.05, 0.0, -42.0),
    vec3 (-0.75, 0.0, -42.0),
    vec3 (-0.45, 0.0, -42.0),
    vec3 (0.45, 0.0, -42.0),
    vec3 (0.75, 0.0, -42.0),
    vec3 (1.05, 0.0, -42.0),
    vec3 (1.35, 0.0, -42.0),
    vec3 (1.65, 0.0, -42.0),
    vec3 (1.95, 0.0, -42.0),

    vec3 (-1.05, 0.0, -36.0),
    vec3 (0.45, 0.0, -35.0),
    vec3 (-1.05, 0.0, -32.0),
    vec3 (-0.75, 0.0, -32.0),
    vec3 (-0.45, 0.0, -32.0),

    vec3 (-0.15, 0.0, -32.0),
    vec3 (0.0, 0.0, -28.0),
    vec3 (0.15, 0.0, -28.0),
    vec3 (0.45, 0.0, -28.0),
    vec3 (0.75, 0.0, -28.0),
    vec3 (-1.05, 0.0, -27.0),
    vec3 (0.0, 0.0, -24.0),
    vec3 (0.15, 0.0, -24.0),
    vec3 (0.45, 0.0, -24.0),
    vec3 (0.75, 0.0, -24.0),
    vec3 (-0.75, 0.0, -23.0),
    vec3 (-1.35, 0.0, -17.0), //prueba
    vec3 (-1.05, 0.0, -17.0),
    vec3 (-0.75, 0.0, -17.0),
    vec3 (-0.45, 0.0, -17.0), //aqui buscar como hacer que vayan vair
    vec3 (0.45, 0.0, -16.0),
    vec3 (0.75, 0.0, -16.0),
    vec3 (1.05, 0.0, -16.0),
    vec3 (-0.75, 0.0, -12.0),
    vec3 (-0.45, 0.0, -12.0),
    vec3 (-0.15, 0.0, -12.0),
    vec3 (0.45, 0.0, -9.0),
    vec3 (0.75, 0.0, -9.0),
    vec3 (1.05, 0.0, -9.0),
    vec3 (-1.05, 0.0, -6.0),
    vec3 (0.45, 0.0, -4.0),
    vec3 (1.05, 0.0, -4.0),
    vec3 (0.0, 0.0, -4.0),
    vec3 (-0.15, 0.0, -4.0)

};

vec3 pos_monedas[] = {

	vec3 (0.3, 0.0, -22.0), //prueba
    /*vec3 (0.5, 0.0, -23.6),
    vec3 (0.5, 0.0, -6.18),
    vec3 (-0.5, 0.0, -30), //aqui buscar como hacer que vayan vair
    vec3 (1.0, 0.0, -10.0),
    vec3 (-1.0, 0.0, -9.0),
    vec3 (1.0, 0.0, -11.0),
    vec3 (1.0, 0.0, -14.0),
    vec3 (0.5, 0.0, -18.0),
    vec3 (0.8, 0.0, -20.0),
    vec3 (-0.8, 0.0, -24.0),
    vec3 (0.3, 0.0, -28.0),
    vec3 (0.0, 0.0, -30.0),
    vec3 (0.5, 0.0, -33.0),
    vec3 (0.0, 0.0, -36.0),
    vec3 (0.0, 0.0, -39.0),
    vec3 (0.4, 0.0, -42.0),

    vec3 (-0.9, 0.0, -45.0),*/

};

vec3 pos_piso[] = {
	vec3(0.0,0.0, 0.0),
	vec3(0.0,0.0, -15.0),
	vec3(0.0,0.0, -30.0)
};

vec3 pos_corazones[] = {
	vec3(-0.9,0.0,-20.0),
	vec3(0.9,0.0,-40.0)
};

int g_gl_width = 1280;
int g_gl_height = 720;
GLFWwindow* g_window = NULL;

int main(){
  init_gl(g_gl_width, g_gl_height);

	GLuint shader_programme = create_programme_from_files ( VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

	// input variables
  mat4 pers_mat = perspective(67.0f, (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
  mat4 ortho_mat = orthographic(-100, 100, 2.5, -2.5, -4, 4);
  // para gameplay con camara
  mat4 proj = pers_mat;
  // para estatico, se ve mejor
  // mat4 proj = ortho_mat;

	float cam_speed = 1.0f; // 1 unit per second
	float cam_yaw_speed = 70.0f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.5f, 1.55f}; //1.55 don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	/* Variables auxiliares que permiten salto, inicio y pausa */
	bool start = false;
	bool jump = false;
	float velocityY = 0.0f;
	float gravity = 0.005f; //orig 0.005
	int auxp = 0;
	int auxv = 0;

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj.m);

  /* Creacion de Objetos */

  personaje *p1 = new personaje((const char*)"mallas/personaje.obj", shader_programme, GL_TRIANGLES);
	p1->load_texture("textures/personaje.png");

	malla *o1 = new malla((const char*)"mallas/obstaculo.obj", shader_programme, GL_TRIANGLES);
	o1->load_texture("textures/obstaculo.jpg");

  /*malla *o [num_obstaculos]; //arreglo de objetos tipo obstaculo
  for (int i = 0; i < num_obstaculos; i++){
    o[i] = new malla((const char*)"mallas/obstaculo.obj", shader_programme, GL_TRIANGLES);
		o[i]->load_texture("textures/obstaculo.jpg");
  }*/

  malla *f [num_piso]; //arreglo de objetos tipo piso
  for(int i = 0; i < num_piso; i++){
    f[i] = new malla((const char*)"mallas/piso.obj", shader_programme, GL_TRIANGLES);
		f[i]->load_texture("textures/piso.jpg");
  }

  malla *m1 = new malla((const char*)"mallas/moneda.obj", shader_programme, GL_TRIANGLES);
	m1->load_texture("textures/moneda.jpg");

  malla *c1 = new malla((const char*)"mallas/corazon.obj", shader_programme, GL_TRIANGLES);
	c1->load_texture("textures/corazon.jpg");

  malla *w1 = new malla((const char*)"mallas/mundo.obj", shader_programme, GL_TRIANGLES);
	w1->load_texture("textures/world3.jpeg");


	/* Ajustes en las matrices de cada objeto, cambio de ubicacion, scale, etc. */

	/*for(int i = 0; i < num_obstaculos; i++){
    o[i]->mtranslate(pos_obstaculos[i]);
  }*/

	mat4 obst_mats[num_obstaculos];
	for (int i = 0; i < num_obstaculos; i++) {
		obst_mats[i] = translate (o1->M, pos_obstaculos[i]);
	}

	mat4 mon_mats[num_monedas];
	for (int i = 0; i < num_monedas; i++) {
		mon_mats[i] = translate (m1->M, pos_monedas[i]);
	}

  for(int i = 0; i < num_piso; i++){
    f[i]->mtranslate(pos_piso[i]);
  }

  //m1->mtranslate(pos_monedas[0]);

  c1->mtranslate(pos_corazones[0]);

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);
		glUseProgram (shader_programme);

    // render caja
    p1->render_indices();

		/*for(int i = 0; i < num_obstaculos; i++){
      o[i]->render_indices();
    }*/

		for(int i = 0; i < num_obstaculos; i++){
			o1->render_indices(obst_mats[i]);
		}

		for(int i = 0; i < num_monedas; i++){
			m1->render_indices(mon_mats[i]);
		}

    for(int i = 0; i < num_piso; i++){
      f[i]->render_indices();
    }


		m1->render_indices();

		c1->render_indices();
		w1->render_indices();

		// update other events like input handling
		glfwPollEvents();
		// control keys
    bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed);

		if (glfwGetKey (g_window, GLFW_KEY_A)){
			if(start){
				cam_pos[0] -= 3*cam_speed * elapsed_seconds;
				p1->M.m[12] -= 0.05f;
				if (p1->M.m[12]< -2.10f && p1->M.m[12]> -2.25f){ //portal A <-
					auxv = p1->getvida();
					auxv = auxv - 1;
					p1->setvida(auxv);
					p1->estadoActual();
					cam_pos[0] = 1.95f;
					p1->M.m[12] = 1.95f;
				}
				cam_moved = true;
			}
		}

		if (glfwGetKey (g_window, GLFW_KEY_D)){
			if(start){
				cam_pos[0] += 3*cam_speed * elapsed_seconds;
				p1->M.m[12] += 0.05f;
				if (p1->M.m[12]> 2.10f && p1->M.m[12]<2.25f){//portal B ->
					auxv = p1->getvida();
					auxv = auxv - 1;
					p1->setvida(auxv);
					p1->estadoActual();
					cam_pos[0] = -1.95f;
					p1->M.m[12] = -1.95f;
				}
				cam_moved = true;
			}
		}

		//pausa
		if (glfwGetKey (g_window, GLFW_KEY_P)) {
			if(start) start = false;
			else if((!start) && p1->M.m[14]< 0.0f && p1->M.m[14]> -45.0f) start = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_F)){//update-camera (focus)
			cam_pos[0] = 0.0f;
			cam_pos[1] = 0.5f;
			cam_yaw = 0.0f;
			cam_moved = true;
		}

		//inicio
		if (glfwGetKey (g_window, GLFW_KEY_ENTER)) {
			if(!start) start = true;
		}

		if (start){
			cam_pos[2] -= 4.5*cam_speed * elapsed_seconds;
			p1->M.m[14] -= 0.075f;
			auxp = p1->getpuntaje();
			auxp += 1;
			p1->setpuntaje(auxp);
			if (p1->M.m[14]<-45.0f || p1->getvida()==0){ //
				cam_pos[0] = 0.0f;//reseteo de la camara
				cam_pos[1] = 0.5f;
				cam_pos[2] = 1.55f;
				p1->M = translate (identity_mat4(), vec3(0.0, 0.0, 0.0));
				p1->setpuntaje(0);
				p1->setvida(3);
				start = false;
			}
			cam_moved = true;
		}

		//salto
		if (glfwGetKey (g_window, GLFW_KEY_SPACE)) {
			if(!jump){
				jump = true;
				velocityY = 0.055; //orig 0.12f
			}
		}
		if (jump){
			cam_pos[1] += velocityY;
			p1->M.m[13] += velocityY;
			velocityY -= gravity;
			if (p1->M.m[13]<0.0f){
				cam_pos[1] = 0.5;
				p1->M.m[13] = 0.0;
				velocityY = 0.0;
				jump = false;
			}
			cam_moved = true;
		}
		// update view matrix
		//printf("cam_pos2 = %f\n", cam_pos[2]);
		if (cam_moved){
      // cam translation
			mat4 T = translate (identity_mat4(), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
      mat4 R = rotate_y_deg(identity_mat4 (), -cam_yaw); //
			mat4 view_mat = R * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
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
