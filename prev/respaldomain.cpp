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

#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
//#define MESH_FILE "monkey2.obj"
//#define MESH_FILE "cosa.obj"
//#define MESH_FILE "mallas/suzanne.obj"

// keep track of window size for things like the viewport and the mouse cursor
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

	float cam_speed = 4.0f; // 1 unit per second
	float cam_yaw_speed = 70.0f; // 10 degrees per second
	float cam_pos[] = {0.0f, 0.0f, 5.0f}; // don't start at zero, or we will be too close
	float cam_yaw = 0.0f; // y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj.m);

  malla *caja = new malla((const char*)"mallas/pancho.obj", shader_programme, GL_TRIANGLES);
  caja->load_texture("textures/pancho.jpg");
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
    caja->reset_matrix();
    caja->render_indices();
    //caja->render_vertices_points(3.0f);

		// update other events like input handling
		glfwPollEvents();
		// control keys
    bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed);
		// update view matrix
		if (cam_moved){
            // cam translation
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
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
