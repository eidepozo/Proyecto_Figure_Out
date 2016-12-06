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
#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"

#include "piso.h"
#include "personaje.h"
#include "obstaculo.h"
#include "moneda.h"
#include "corazon.h"
#define MESH_FILE "mallas/piso.obj"
#define MESH_FILE1 "mallas/personaje2.obj"
#define MESH_FILE2 "mallas/obstaculo.obj"
#define MESH_FILE3 "mallas/moneda.obj"
#define MESH_FILE4 "mallas/corazon.obj"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 1280;
int g_gl_height = 690;
GLFWwindow* g_window = NULL;

int main(){
	init(); /* Define elementos basicos de OpenGL, se ubica en tools.cpp */
		
    /* Creacion de Objetos */
    personaje *p1 = new personaje((char*)"mallas/personaje.obj");
    obstaculo *o1 = new obstaculo((char*)"mallas/obstaculo.obj");
    obstaculo *o2 = new obstaculo((char*)"mallas/obstaculo.obj");
    obstaculo *o3 = new obstaculo((char*)"mallas/obstaculo.obj");
    //obstaculo *o4 = new obstaculo((char*)"mallas/obstaculo.obj");
    moneda *m1 = new moneda((char*)"mallas/moneda.obj");
    corazon *c1 = new corazon((char*)"mallas/corazon.obj");
    piso *f1 = new piso((char*)"mallas/piso.obj");
	
//-------------------------------CREATE SHADERS-------------------------------
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);
	
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

	/* Ajustes en las matrices de cada objeto, cambio de ubicacion, scale, etc. */
	p1->M = translate (identity_mat4(), vec3(0.0,-0.15,5.75));

	o1->M = translate (identity_mat4(), vec3(1.5,-0.15,2.5));//al lado del corazon
	o2->M = translate (identity_mat4(), vec3(-2.0,-0.15,3.75));
	o3->M = translate (identity_mat4(), vec3(-1.25,-0.15,3.75));
	//o4->M = translate (identity_mat4(), vec3(1.25,-0.15,2.0));
	
	m1->M = translate (identity_mat4(), vec3(-1.65,-0.15,3.75));
	
	c1->M = translate (identity_mat4(), vec3(2.0,0.2,5.0));
	
	f1->M = scale (f1->M, vec3(5.0,0.0,15.0));
	f1->M = translate (identity_mat4(), vec3(0.0,-0.35,-0.15));

	float cam_speed = 1.0f; //1.0 unites per second
	float cam_yaw_speed = 30.0f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.1f, 7.5f}; // og 5.0 don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees //0.0 0.0 5.0
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	/* Variables auxiliares que permiten salto, inicio y pausa */
	bool start = false;
	bool jump = false;
	float velocityY = 0.0f;
	float gravity = 0.005f;
	
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

	/* Llamada al programa shader, y envio de las matrices */
	int p1_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (p1_mat_location, 1, GL_FALSE, p1->M.m);

	int o1_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (o1_mat_location, 1, GL_FALSE, o1->M.m);

	int o2_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (o2_mat_location, 1, GL_FALSE, o2->M.m);

	int o3_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (o3_mat_location, 1, GL_FALSE, o3->M.m);

	/*int o4_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (o4_mat_location, 1, GL_FALSE, o4->M.m);*/
	
	int m1_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (m1_mat_location, 1, GL_FALSE, m1->M.m);

	int c1_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (c1_mat_location, 1, GL_FALSE, c1->M.m);

	int f1_mat_location = glGetUniformLocation (shader_programme, "obj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (f1_mat_location, 1, GL_FALSE, f1->M.m);

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

		//personaje, piso, obstaculo, moneda, corazon
		glUniformMatrix4fv (p1_mat_location, 1 ,GL_FALSE, p1->M.m);
        glBindVertexArray(p1->getvao());
        glDrawArrays(GL_TRIANGLES,0,p1->getnumvertices());
		
		glUniformMatrix4fv (o1_mat_location, 1 ,GL_FALSE, o1->M.m);
        glBindVertexArray(o1->getvao());
        glDrawArrays(GL_TRIANGLES,0,o1->getnumvertices());

        glUniformMatrix4fv (o2_mat_location, 1 ,GL_FALSE, o2->M.m);
        glBindVertexArray(o2->getvao());
        glDrawArrays(GL_TRIANGLES,0,o2->getnumvertices());

        glUniformMatrix4fv (o3_mat_location, 1 ,GL_FALSE, o3->M.m);
        glBindVertexArray(o3->getvao());
        glDrawArrays(GL_TRIANGLES,0,o3->getnumvertices());

        /*glUniformMatrix4fv (o4_mat_location, 1 ,GL_FALSE, o4->M.m);
        glBindVertexArray(o4->getvao());
        glDrawArrays(GL_TRIANGLES,0,o4->getnumvertices());*/

        glUniformMatrix4fv (m1_mat_location, 1 ,GL_FALSE, m1->M.m);
        glBindVertexArray(m1->getvao());
        glDrawArrays(GL_TRIANGLES,0,m1->getnumvertices());

        glUniformMatrix4fv (c1_mat_location, 1 ,GL_FALSE, c1->M.m);
        glBindVertexArray(c1->getvao());
        glDrawArrays(GL_TRIANGLES,0,c1->getnumvertices());

        glUniformMatrix4fv (f1_mat_location, 1 ,GL_FALSE, f1->M.m);
       	glBindVertexArray(f1->getvao());
        glDrawArrays(GL_TRIANGLES,0,f1->getnumvertices());

		// update other events like input handling 
		glfwPollEvents ();
		
		/* Configuracion de la camara e interaccion-teclado */
	    bool cam_moved = false;

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

		if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)){
			cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)){
			cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_W)){
			cam_pos[2] -= 3*cam_speed * elapsed_seconds;
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_S)){
			cam_pos[2] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_LEFT)){
			cam_yaw += cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_RIGHT)){
			cam_yaw -= cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}

		//pausa
		if (glfwGetKey (g_window, GLFW_KEY_P)) {
			if(start) start = false;

			else if((!start) && cam_pos[2]<6.0f && cam_pos[2]>-5.0f) start = true;
		}

		if (glfwGetKey (g_window, GLFW_KEY_F)){//update-camera (focus)
			cam_pos[0] = 0.0f;
			cam_pos[1] = 0.1f;
			cam_pos[2] = 7.5f;
			cam_yaw = 0.0f;
			p1->M = translate (identity_mat4(), vec3(0.0,-0.15,5.75));
			cam_moved = true;
		}

		//inicio
		if (glfwGetKey (g_window, GLFW_KEY_ENTER)) {
			//printf ("inicio\n");
			if(!start) start = true;
		}

		if (start){
			cam_pos[2] -= 3.0*cam_speed * elapsed_seconds;
			p1->M.m[14] -= 0.05f;
			if (cam_pos[2]<-5.0f){ // largo
				cam_pos[0] = 0.0f;//reseteo de la camara
				cam_pos[1] = 0.1f;
				cam_pos[2] = 7.5f; 
				p1->M = translate (identity_mat4(), vec3(0.0,-0.15,5.75));
				//cam_speed = 1.0f;
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

		//control auxiliar de la camara
   		if (glfwGetKey (g_window, GLFW_KEY_J)){
			cam_pos[0] -= 3*cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		
		if (glfwGetKey (g_window, GLFW_KEY_L)){
			cam_pos[0] += 3*cam_speed * elapsed_seconds;
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
			//nos permite pasar una matriz al shader
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
