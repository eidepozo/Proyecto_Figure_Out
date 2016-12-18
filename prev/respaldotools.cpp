#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "maths_funcs.h"
#include "gl_utils.h"
#include "tools.h"

#define PI 3.14159265359
#define num_obstaculos 64 //macros
#define num_piso 3
#define num_monedas 16
#define num_corazones 2

void init_gl(int width, int height){
	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
  glEnable( GL_POINT_SMOOTH );
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor (1.0, 1.0, 1.0, 1.0); // grey background to help spot mistakes
	glViewport (0, 0, width, height);
}

bool gameplay(float cam_speed, double elapsed_seconds, float *cam_pos, float *cam_yaw, float cam_yaw_speed){
    bool cam_moved = false;
    float ryaw = *cam_yaw*PI/180.0;
    if (glfwGetKey (g_window, GLFW_KEY_J)) {
        cam_pos[0] -= cos(ryaw)* cam_speed * elapsed_seconds;
        cam_pos[2] += sin(ryaw)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_L)){
        cam_pos[0] += cos(ryaw)* cam_speed * elapsed_seconds;
        cam_pos[2] -= sin(ryaw)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_PAGE_UP)) {
        cam_pos[1] += cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_PAGE_DOWN)) {
        cam_pos[1] -= cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_I)) {
        cam_pos[0] -= sin(ryaw)* 5*cam_speed * elapsed_seconds;
        cam_pos[2] -= cos(ryaw)* 5*cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_K)) {
        cam_pos[0] += sin(ryaw)* cam_speed * elapsed_seconds;
        cam_pos[2] += cos(ryaw)* cam_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_LEFT)) {
        *cam_yaw += cam_yaw_speed * elapsed_seconds;
        cam_moved = true;
    }

    if (glfwGetKey (g_window, GLFW_KEY_RIGHT)) {
        *cam_yaw -= cam_yaw_speed * elapsed_seconds;
        cam_moved = true;
    }

    return cam_moved;
}

vec3* ubica_obstaculos(){
	vec3* pobst = (vec3 *)malloc(sizeof(vec3) * num_obstaculos);
	pobst[63] = vec3(1.5, 0.0, -1.5);
	pobst[0] = vec3(-0.15, 0.0, -4.05);
	pobst[1] = vec3(0.0, 0.0, -4.05); //con este cubo es la colision
	pobst[2] = vec3(0.45, 0.0, -4.05);
	pobst[3] = vec3(1.05, 0.0, -4.05);
	pobst[4] = vec3(-1.05, 0.0, -6.0);

	pobst[5] = vec3(-0.45, 0.0, -7.95);//tres cajas esta es la que hace problema

	pobst[6] = vec3(-0.2, 0.0, -7.95);//dos juntas v
	pobst[7] = vec3(0.0, 0.0, -7.95);//Nope

	pobst[8] = vec3(0.45, 0.0, -9.0);
	pobst[9] = vec3(0.75, 0.0, -9.0);
	pobst[10] = vec3(1.05, 0.0, -9.0);
	pobst[11] = vec3(-0.75, 0.0, -12.0);
	pobst[12] = vec3(-0.45, 0.0, -12.0);
	pobst[13] = vec3(-0.15, 0.0, -12.0);
	pobst[14] = vec3(0.0, 0.0, -15.0);


	pobst[15] = vec3(0.45, 0.0, -16.05);
	pobst[16] = vec3(0.75, 0.0, -16.05);
	pobst[17] = vec3(1.05, 0.0, -16.05);
	pobst[18] = vec3(-1.35, 0.0, -16.95);
	pobst[19] = vec3(-1.05, 0.0, -16.95);
	pobst[20] = vec3(-0.75, 0.0, -16.95);
	pobst[21] = vec3(-0.45, 0.0, -16.95);

	pobst[22] = vec3(-0.75, 0.0, -22.95);
	pobst[23] = vec3(0.45, 0.0, -22.95);
	pobst[24] = vec3(0.75, 0.0, -22.95);
	pobst[25] = vec3(0.0, 0.0, -24.0);
	pobst[26] = vec3(0.15, 0.0, -24.0);
	pobst[27] = vec3(0.45, 0.0, -24.0);
	pobst[28] = vec3(0.75, 0.0, -24.0);
	pobst[29] = vec3(-0.15, 0.0, -25.95);
	pobst[30] = vec3(-0.45, 0.0, -25.95);
	pobst[31] = vec3(-1.05, 0.0, -27.0);
	pobst[32] = vec3(0.0, 0.0, -28.05);
  pobst[33] = vec3(0.15, 0.0, -28.05);
	pobst[34] = vec3(0.45, 0.0, -28.05);
	pobst[35] = vec3(0.75, 0.0, -28.05);

	pobst[36] = vec3(-1.05, 0.0, -31.95);
	pobst[37] = vec3(-0.75, 0.0, -31.95);
	pobst[38] = vec3(-0.45, 0.0, -31.95);
	pobst[39] = vec3(-0.15, 0.0, -31.95);
	pobst[40] = vec3(-0.15, 0.0, -34.95);
	pobst[41] = vec3(0.0, 0.0, -34.95);
	pobst[42] = vec3(0.45, 0.0, -34.95);
	pobst[43] = vec3(-1.05, 0.0, -36.0);
	pobst[44] = vec3(0.0, 0.0, -37.95);
	pobst[45] = vec3(0.15, 0.0, -37.95);
	pobst[46] = vec3(0.45, 0.0, -37.95);
	pobst[47] = vec3(0.75, 0.0, -37.95);
	pobst[48] = vec3(1.05, 0.0, -37.95);

	pobst[49] = vec3(-0.15, 0.0, -40.05);
	pobst[50] = vec3(0.0, 0.0, -40.05);
	pobst[51] = vec3(-1.95, 0.0, -42.0);
	pobst[52] = vec3(-1.65, 0.0, -42.0);
	pobst[53] = vec3(-1.35, 0.0, -42.0);
	pobst[54] = vec3(-1.05, 0.0, -42.0);
	pobst[55] = vec3(-0.75, 0.0, -42.0);
	pobst[56] = vec3(-0.45, 0.0, -42.0);
	pobst[57] = vec3(0.45, 0.0, -42.0);
	pobst[58] = vec3(0.75, 0.0, -42.0);
	pobst[59] = vec3(1.05, 0.0, -42.0);
	pobst[60] = vec3(1.35, 0.0, -42.0);
	pobst[61] = vec3(1.65, 0.0, -42.0);
	pobst[62] = vec3(1.95, 0.0, -42.0);
	return pobst;
}

vec3* ubica_monedas(){
	vec3* pmon = (vec3 *)malloc(sizeof(vec3) * num_monedas);
	pmon[0] = vec3(0.15, 0.0, -0.064);
	pmon[1] = vec3(0.15, 0.0, -6.989);
	pmon[2] = vec3(0.45, 0.0, -11.989);
	pmon[3] = vec3(0.0, 0.0, -18.989);
	pmon[4] = vec3(-1.35, 0.0, -21.989);
	pmon[5] = vec3(-1.35, 0.0, -22.989);
	pmon[6] = vec3(-1.35, 0.0, -24.989);
	pmon[7] = vec3(-0.45, 0.0, -28.989);
	pmon[8] = vec3(0.0, 0.0, -32.989);
	pmon[9] = vec3(1.05, 0.0, -32.989);
	pmon[10] = vec3(1.05, 0.0, -33.989);
	pmon[11] = vec3(1.05, 0.0, -34.989);
	pmon[12] = vec3(-0.45, 0.0, -37.989);
	pmon[13] = vec3(-0.15, 0.0, -37.989);
	pmon[14] = vec3(-0.15, 0.0, -38.989);
	pmon[15] = vec3(0.0, 0.0, -44.989);
	return pmon;
}

vec3* ubica_piso(){
	vec3* ppiso = (vec3 *)malloc(sizeof(vec3)* num_piso);
	ppiso[0]= vec3(0.0, 0.0, 0.0);
	ppiso[1]= vec3(0.0, 0.0, -15.0);
	ppiso[2]= vec3(0.0, 0.0, -30.0);
	return ppiso;
}

vec3* ubica_corazones(){
	vec3* pcor = (vec3 *)malloc(sizeof(vec3)* num_corazones);
	pcor[0]= vec3(1.05, 0.0, -24.0);
	pcor[1]= vec3(-1.35, 0.0, -34.0);
	return pcor;
}

bool manejador_colisiones(float px, float pz, mat4* obst_mats){
	//printf("pz = %f\n", pz);
	//printf("px = %f\n", px);
	for(int i = 0; i < 15; i++){
		float auxx = obst_mats[i].m[12] - px;
		float auxz = obst_mats[i].m[14] - pz;
		//printf("auxx = %f\n",auxx);
		//printf("auxz = %f\n",auxz);
		if (auxz>0 && auxz <0.15 && auxx>-0.15 && auxx<0.15){
			printf("Colision \n");
			return true;
		}
	}
	return false;
}
