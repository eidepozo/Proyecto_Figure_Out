#ifndef TOOLS_H
#define TOOLS_H

// init GL
void init_gl(int width, int height);

// funcion que carga una malla desde filename
bool load_mesh (const char* file_name, GLuint* vao, GLuint* point_count, GLuint *face_count, GLuint *face_factor);
// funcion gameplay
bool gameplay(float cam_speed, double elapsed_seconds, float *cam_pos, float *cam_yaw, float cam_yaw_speed);
#endif

vec3* ubica_obstaculos();
vec3* ubica_monedas();
vec3* ubica_piso();
vec3* ubica_corazones();
bool manejador_colisionesA(float px, float py, float pz, vec3* pobs, int pvi, int pvf);
bool manejador_colisionesB(float px, float py, float pz, vec3* pmon, int pvi, int pvf);
bool manejador_colisionesC(float px, float py, float pz, vec3* pcor, int pvi, int pvf);
