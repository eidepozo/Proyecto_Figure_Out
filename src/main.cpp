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
#define num_obstaculos 265 //macros 63 + 16
#define num_piso 3
#define num_monedas 60
#define num_corazones 1

/* Llamadas a las respectivas funciones que definen la posicion de las mallas (ubicada en tools.cpp) */
vec3* pos_obstaculos = ubica_obstaculos();
vec3* pos_monedas = ubica_monedas();
vec3* pos_piso = ubica_piso();
vec3 * pos_corazones = ubica_corazones();

int g_gl_width = 1280;
int g_gl_height = 720;
GLFWwindow* g_window = NULL;

int main(){
  init_gl(g_gl_width, g_gl_height); /* Funcion que define parametros basicos de OpenGL (ubicada en tools.cpp) */

	GLuint shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

	// input variables
  mat4 pers_mat = perspective(67.0f, (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
  mat4 ortho_mat = orthographic(-100, 100, 2.5, -2.5, -4, 4);
  // para gameplay con camara
  mat4 proj = pers_mat;
  // para estatico, se ve mejor
  // mat4 proj = ortho_mat;

	float cam_speed = 1.0f; // 1 unit per second
	float cam_yaw_speed = 70.0f; // 70 degrees per second
	float cam_pos[] = {0.0f, 0.5f, 1.55f}; //0.0, 0.0, 5.0 don't start at zero, or we will be too close 1.55
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	/* Variables auxiliares que permiten controlar las interacciones del personaje con el entorno, y el flujo
  del juego */
	bool start, jump, colA, colB, colC = false;
	float velocityY = 0.0f;
	float gravity = 0.005f;
	int auxp ,auxv, auxs = 0;


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

  malla *f1 = new malla((const char*)"mallas/piso.obj", shader_programme, GL_TRIANGLES);
  f1->load_texture("textures/piso.jpg");

  malla *m1 = new malla((const char*)"mallas/moneda.obj", shader_programme, GL_TRIANGLES);
	m1->load_texture("textures/moneda.jpg");

  malla *c1 = new malla((const char*)"mallas/corazon.obj", shader_programme, GL_TRIANGLES);
	c1->load_texture("textures/corazon.jpg");

  malla *w1 = new malla((const char*)"mallas/mundo.obj", shader_programme, GL_TRIANGLES);
	w1->load_texture("textures/texture2.jpg");

	/* Arreglo de matrices para cada objeto */
	mat4 obst_mats[num_obstaculos];
  mat4 mon_mats[num_monedas];
  mat4 piso_mats[num_piso];
  mat4 cor_mats[num_piso];

  /* Traslacion de las matrices para el rendering */
  //p1->M = translate (identity_mat4(), vec3(-0.29, 0.0, 0.0)); testing
	for (int i = 0; i < num_obstaculos; i++) obst_mats[i] = translate (o1->M, pos_obstaculos[i]);
	for (int i = 0; i < num_monedas; i++) mon_mats[i] = translate (m1->M, pos_monedas[i]);
  for (int i = 0; i < num_piso; i++) piso_mats[i] = translate (f1->M, pos_piso[i]);
  for (int i = 0; i < num_corazones; i++) cor_mats[i] = translate (c1->M, pos_corazones[i]);

  printf("Proyecto Figure Out Ver. 1.2.3\nAutores: Elliot Ide Pozo, Adolfo Cañoles, Rodrigo Miranda.\n<Enter> para iniciar.\n<Esc> para salir.\n");

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

    /* Rendering */
    p1->render_indices();
		for(int i = 0; i < num_obstaculos; i++) o1->render_indices(obst_mats[i]);
		for(int i = 0; i < num_monedas; i++) m1->render_indices(mon_mats[i]);
    for(int i = 0; i < num_piso; i++) f1->render_indices(piso_mats[i]);
    for(int i = 0; i < num_corazones; i++) c1->render_indices(cor_mats[i]);
		w1->render_indices();

		// update other events like input handling
		glfwPollEvents();

    /* Llamada a la funcion que controla a la "camara 2", (ubicada en tools.cpp) */
    bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed);

    /* Interaccion teclado con la "camara 1" */

    /* Cuando llego a uno de los extremos horizontales, paso hacia el extremo opuesto
    perdiendo una vida*/
		if (glfwGetKey (g_window, GLFW_KEY_A)){ //movimiento hacia la izda
			if(start){
				cam_pos[0] -= 3*cam_speed * elapsed_seconds;
				p1->M.m[12] -= 0.05f;
        //printf("p1->M.m[12] = %f\n", p1->M.m[12]); px
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

		if (glfwGetKey (g_window, GLFW_KEY_D)){//movimiento hacia la derecha
			if(start){
				cam_pos[0] += 3*cam_speed * elapsed_seconds;
				p1->M.m[12] += 0.05f;
        //printf("p1->M.m[12] = %f\n", p1->M.m[12]); px
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

    /* Movimientos auxiliares para testing */
    /*if (glfwGetKey (g_window, GLFW_KEY_W)){//auxiliar
      if(start){
        p1->M.m[14] -= 0.075f;
        printf("p1->M.m[14] = %f\n", p1->M.m[14]); //pz
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_S)){//auxiliar
      //if(start){
        p1->M.m[14] += 0.075f;
        printf("p1->M.m[14] = %f\n", p1->M.m[14]); //pz
        cam_moved = true;
    }
    */

    /* Puedo pausar el juego, volviendo la variable de inicio (start) false, solo si
    estoy en los margenes del eje z donde se puede jugar */
		if (glfwGetKey (g_window, GLFW_KEY_P)){//pausa
			if(start) start = false;
			else if((!start) && p1->M.m[14]< 0.0f && p1->M.m[14]> -45.0f) start = true;
		}

    /* Reinicio los valores de la camara, si no se esta jugando, se incluye la posicion en el eje z*/
		if (glfwGetKey (g_window, GLFW_KEY_F)){//update-camera (focus)
			cam_pos[0] = 0.0f;
			cam_pos[1] = 0.5f;
      cam_yaw = 0.0f;
      if(!start){
        cam_pos[2] = 1.55f;
      }
			cam_moved = true;
		}

    /* Inicio del juego, las demas interacciones con teclado requieren que se realize esta interaccion primero */
		if (glfwGetKey (g_window, GLFW_KEY_ENTER)){
			if(!start) start = true;
		}

    /* Cuando el juego inicia, constantemente se analiza el estado del juego y del personaje */
		if (start){
      /* El personaje se va desplazando por el eje z al igual que la camara, y su puntaje se va incrementando
      y actualizando */
			cam_pos[2] -= 4.5*cam_speed * elapsed_seconds;
			p1->M.m[14] -= 0.075f; //0.075
      //printf("p1->M.m[14] = %f\n", p1->M.m[14]); pz
			auxp = p1->getpuntaje();
			auxp += 1;
      p1->estadoActual();
			p1->setpuntaje(auxp);

      /* Las colisiones se evaluan en 3 secciones de 15 unidades(largopiso/3) para disminuir el numero de
      casos que se deben analizar a la vez.
      Evaluamos en que seccion esta ubicado el personaje viendo su posicion en el eje z, y le entregamos a las funciones
      que manejan las colisiones, su posicion en los 3 ejes, el arreglo de posiciones respectivo, con un rango que le
      indica que casos se deben analizar */
      if(p1->M.m[14]<=0.0f && p1->M.m[14]>=-15.0f){//seccion 1
        colA = manejador_colisionesA(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_obstaculos, 0, 79);// pvf+1
        colB = manejador_colisionesB(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_monedas, 0, 16);
      } /*por ejemplo, aqui en el arreglo de obstaculos, los obstaculos que estan en el eje z entre 0 y -15
      estan entre las posiciones 0 y 78 del arreglo (pobst)*/

      else if(p1->M.m[14]<-15.0f && p1->M.m[14]>=-30.0f){//seccion 2
        colA = manejador_colisionesA(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_obstaculos, 79, 175);
        colB = manejador_colisionesB(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_monedas, 16, 41);
        colC = manejador_colisionesC(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_corazones, 0, 1);
      }

      else if(p1->M.m[14]<-30.0f && p1->M.m[14]>=-45.0f){//seccion 3
        colA = manejador_colisionesA(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_obstaculos, 175, 247);
        colB = manejador_colisionesB(p1->M.m[12], p1->M.m[13], p1->M.m[14],pos_monedas, 41, 60);
      }

      /*Cuando ocurra una colision con un obstaculo (tipo A), la vida se disminuira en 1 y el puntaje volvera a 0,
      lo que es diferente a cuando pasamos por un extremo y el juego sigue, las colisiones A, toman las
      vidas como intentos del juego */
      if (colA){
        auxv = p1->getvida();
        auxv = auxv - 1;
        p1->setvida(auxv);
        p1->setpuntaje(0);
        p1->estadoActual();
      }

      /* Cuando hay una colision con una moneda (tipo B), el puntaje aumentara en 50, y se reinicia el estado de colB
      para las siguientes monedas */
      if (colB){
        auxp = p1->getpuntaje();
        auxp = auxp + 50;
        p1->setpuntaje(auxp);
        colB = false;
      }

      /* Y cuando se colisiona con un corazon, la vida aumentara en 1, y el puntaje en 100, y se reinicia el estado de colC
      para los siguientes corazones*/
      if (colC){
        auxp = p1->getpuntaje();
        auxp = auxp + 100;
        p1->setpuntaje(auxp);
        auxv = p1->getvida();
        auxv = auxv +1;
        p1->setvida(auxv);
        colC = false;
      }

      /* Casos criticos: Si ocurre una colision tipo A, o termino el juego (posicion en el eje Z < -45)
      , o no quedan vidas restantes*/
			if (colA || p1->M.m[14]<-45.0f || p1->getvida()==0){
      /* Para los 3 casos, pongo la camara en su posicion original, pero debo ver si estoy en un caso
      que indica el termino del juego para reiniciar los valores propios del personaje*/
        cam_pos[0] = 0.0f;
				cam_pos[1] = 0.5f;
				cam_pos[2] = 1.55f;
        if(p1->M.m[14]<-45.0f || p1->getvida()==0){//GAME OVER
          printf("GAME OVER\n");
          p1->setpuntaje(0);
				  p1->setvida(3);
          p1->setnsaltos(3);
        }
        /* Posteriormente, reinicio la posicion del personaje, el estado del juego y el estado de colA */
        p1->M = translate (identity_mat4(), vec3(0.0, 0.0, 0.0));
				start = false;
        colA = false;
			}
			cam_moved = true;
		}

    /* Inicialmente tengo 3 saltos, cada vez que salte debo actualizar ese valor, ademas
    solo puedo saltar si tengo saltos y si no estoy saltando ya mismo y si start es verdadero*/
		if (glfwGetKey (g_window, GLFW_KEY_SPACE)){//salto
      auxs = p1->getnsaltos();
			if(start && !jump && auxs > 0){
				jump = true;
        auxs = auxs - 1;
        //printf("nsaltos = %i\n", auxs);
        p1->setnsaltos(auxs);
				velocityY = 0.055;
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
