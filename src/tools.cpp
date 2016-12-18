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
#define num_obstaculos 265
#define num_piso 3
#define num_monedas 60
#define num_corazones 1

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

/* Todas las mallas en blender se apoyan en los origenes para z e y, y para x estan centradas en el origen.
Esto resulta en que cuando ubicamos un obstaculo en el eje z o en el eje y, digamos en 0.15, se crea desde ese espacio
hacia adelante en esos ejes, y si indicamos que la posicion de un obstaculo en el eje x es 0.0, la malla estara centrada
en el 0.0 para el eje x y ocupara su espacio en torno a esa ubicacion.

El personaje, y los obstaculos tienen las mismas dimensiones (0.15), y el piso es de ancho 4.5 y largo 45
(30 cubos en el eje x, y 300 cubos en el eje z). Por lo que para ubicar un obstaculo, las posiciones para los 3 ejes
son un multiplo de 0.15 (+ o - para x, + para y, - para z) */

vec3* ubica_obstaculos(){
	vec3* pobst = (vec3 *)malloc(sizeof(vec3) * num_obstaculos);
	/* [Seccion 1] */

	//xx--xx-------(-)-------xx--xx

	pobst[0] = vec3(-1.65, 0.0, -2.85);
	pobst[1] = vec3(-1.5, 0.0, -2.85);
	pobst[2] = vec3(-1.05, 0.0, -2.85);
	pobst[3] = vec3(-0.9, 0.0, -2.85);
	pobst[4] = vec3(-0.75, 0.0, -2.85);
	pobst[5] = vec3(-0.6, 0.0, -2.85);
	pobst[6] = vec3(-0.45, 0.0, -2.85);
	pobst[7] = vec3(-0.3, 0.0, -2.85);
	pobst[8] = vec3(-0.15, 0.0, -2.85);
	pobst[9] = vec3(0.0, 0.0, -2.85);
	pobst[10] = vec3(0.15, 0.0, -2.85);
	pobst[11] = vec3(0.3, 0.0, -2.85);
	pobst[12] = vec3(0.45, 0.0, -2.85);
	pobst[13] = vec3(0.6, 0.0, -2.85);
	pobst[14] = vec3(0.75, 0.0, -2.85);
	pobst[15] = vec3(0.9, 0.0, -2.85);
	pobst[16] = vec3(1.05, 0.0, -2.85);
	pobst[17] = vec3(1.5, 0.0, -2.85);
	pobst[18] = vec3(1.65, 0.0, -2.85);

	//xxxx-xxxxxxxx(x)xxxxxxxx-xxxx
	pobst[19] = vec3(-1.5, 0.0, -3.0);
	pobst[20] = vec3(1.5, 0.0, -3.0);

	//xxxxxxxx--xx-(-)-xx--xxxxxxxx
	pobst[21] = vec3(-0.75, 0.0, -3.75);
	pobst[22] = vec3(-0.6, 0.0, -3.75);
	pobst[23] = vec3(-0.15, 0.0, -3.75);
	pobst[24] = vec3(0.0, 0.0, -3.75);
	pobst[25] = vec3(0.15, 0.0, -3.75);
	pobst[26] = vec3(0.6, 0.0, -3.75);
	pobst[27] = vec3(0.75, 0.0, -3.75);

	//xxxxxxxxxx--x(x)x--xxxxxxxxxx
	pobst[28] = vec3(-0.45, 0.0, -4.5);
	pobst[29] = vec3(-0.3, 0.0, -4.5);
	pobst[30] = vec3(0.3, 0.0, -4.5);
	pobst[31] = vec3(0.45, 0.0, -4.5);

	//xxxx-xxxxxxxx(x)xxxxxxxx-xxxx
	pobst[32] = vec3(-1.2, 0.0, -4.65);
	pobst[33] = vec3(1.2, 0.0, -4.65);

	//xxxxxxxxxxxx-(-)-xxxxxxxxxxxx
	pobst[34] = vec3(-0.15, 0.0, -6.75);
	pobst[35] = vec3(0.0, 0.0, -6.75);
	pobst[36] = vec3(0.15, 0.0, -6.75);

	/* Antes de la V */
	//x-xx-----x---(x)---x-----xx-x
	pobst[37] = vec3(-1.8, 0.0, -11.1);
	pobst[38] = vec3(-1.35, 0.0, -11.1);
	pobst[39] = vec3(-1.2, 0.0, -11.1);
	pobst[40] = vec3(-1.05, 0.0, -11.1);
	pobst[41] = vec3(-0.9, 0.0, -11.1);
	pobst[42] = vec3(-0.75, 0.0, -11.1);

	pobst[43] = vec3(-0.45, 0.0, -11.1);
	pobst[44] = vec3(-0.3, 0.0, -11.1);
	pobst[45] = vec3(-0.15, 0.0, -11.1);
	pobst[46] = vec3(0.0, 0.0, -11.1);
	pobst[47] = vec3(0.15, 0.0, -11.1);
	pobst[48] = vec3(0.3, 0.0, -11.1);
	pobst[49] = vec3(0.45, 0.0, -11.1);

	pobst[50] = vec3(0.75, 0.0, -11.1);
	pobst[51] = vec3(0.9, 0.0, -11.1);
	pobst[52] = vec3(1.05, 0.0, -11.1);
	pobst[53] = vec3(1.2, 0.0, -11.1);
	pobst[54] = vec3(1.35, 0.0, -11.1);
	pobst[55] = vec3(1.8, 0.0, -11.1);

	//xxxxxxxxxx-xx(x)xx-xxxxxxxxxx
	pobst[56] = vec3(-0.6, 0.0, -11.55);
	pobst[57] = vec3(0.6, 0.0, -11.55);

	/* V */
	pobst[58] = vec3(0.0, 0.0, -12.75);

	pobst[59] = vec3(-0.15, 0.0, -12.9);
	pobst[60] = vec3(0.15, 0.0, -12.9);

	pobst[61] = vec3(-0.3, 0.0, -13.05);
	pobst[62] = vec3(0.3, 0.0, -13.05);

	pobst[63] = vec3(-0.45, 0.0, -13.2);
	pobst[64] = vec3(0.45, 0.0, -13.2);

	pobst[65] = vec3(-0.6, 0.0, -13.35);
	pobst[66] = vec3(0.6, 0.0, -13.35);

	pobst[67] = vec3(-0.75, 0.0, -13.5);
	pobst[68] = vec3(0.75, 0.0, -13.5);

	pobst[69] = vec3(-0.9, 0.0, -13.65);
	pobst[70] = vec3(0.9, 0.0, -13.65);

	pobst[71] = vec3(-1.05, 0.0, -13.8);
	pobst[72] = vec3(1.05, 0.0, -13.8);

	pobst[73] = vec3(-1.65, 0.0, -14.4);
	pobst[74] = vec3(1.65, 0.0, -14.4);

	pobst[75] = vec3(-1.5, 0.0, -14.55);
	pobst[76] = vec3(1.5, 0.0, -14.55);

	pobst[77] = vec3(-1.5, 0.0, -14.7);
	pobst[78] = vec3(1.5, 0.0, -14.7);

	/* [Seccion 2] */

	//---x---x--x-x(x)x-x--x---x---
	pobst[79] = vec3(-1.95, 0.0, -16.2);
	pobst[80] = vec3(-1.8, 0.0, -16.2);
	pobst[81] = vec3(-1.65, 0.0, -16.2);
	pobst[82] = vec3(-1.35, 0.0, -16.2);
	pobst[83] = vec3(-1.2, 0.0, -16.2);
	pobst[84] = vec3(-1.05, 0.0, -16.2);
	pobst[85] = vec3(-0.75, 0.0, -16.2);
	pobst[86] = vec3(-0.6, 0.0, -16.2);
	pobst[87] = vec3(-0.3, 0.0, -16.2);
	pobst[88] = vec3(0.3, 0.0, -16.2);
	pobst[89] = vec3(0.6, 0.0, -16.2);
	pobst[90] = vec3(0.75, 0.0, -16.2);
	pobst[91] = vec3(1.05, 0.0, -16.2);
	pobst[92] = vec3(1.2, 0.0, -16.2);
	pobst[93] = vec3(1.35, 0.0, -16.2);
	pobst[94] = vec3(1.65, 0.0, -16.2);
	pobst[95] = vec3(1.8, 0.0, -16.2);
	pobst[96] = vec3(1.95, 0.0, -16.2);

	/* Antes de la cruz */
	//xxxxxxxxxxxxx(-)xxxxxxxxxxxxx
	pobst[97] = vec3(0.0, 0.0, -17.85);

	//xxxxx-xx-xxxx(x)xx-xx-xxxxxxx
	pobst[98] = vec3(-1.05, 0.0, -19.65);
	pobst[99] = vec3(-0.6, 0.0, -19.95);
	pobst[100] = vec3(0.6, 0.0, -19.95);
	pobst[101] = vec3(1.05, 0.0, -19.65);


	/* Margen */
	//--xx---xxxxxx(x)xxxxxx---xx--
	pobst[102] = vec3(-1.95, 0.0, -21.0);
	pobst[103] = vec3(-1.8, 0.0, -21.0);

	pobst[104] = vec3(-1.35, 0.0, -21.0);
	pobst[105] = vec3(-1.2, 0.0, -21.0);
	pobst[106] = vec3(-1.05, 0.0, -21.0);

	pobst[107] = vec3(1.05, 0.0, -21.0);
	pobst[108] = vec3(1.2, 0.0, -21.0);
	pobst[109] = vec3(1.35, 0.0, -21.0);

	pobst[110] = vec3(1.8, 0.0, -21.0);
	pobst[111] = vec3(1.95, 0.0, -21.0);


	/* Cruz 1 */
	pobst[112] = vec3(0.0, 0.0, -21.75);// |
	pobst[113] = vec3(0.0, 0.0, -22.05);// x
	pobst[114] = vec3(0.0, 0.0, -22.35);// |
	pobst[115] = vec3(0.0, 0.0, -22.65);

	//-------------(-)
	pobst[116] = vec3(-1.95, 0.0, -22.95);
	pobst[117] = vec3(-1.8, 0.0, -22.95);
	pobst[118] = vec3(-1.65, 0.0, -22.95);
	pobst[119] = vec3(-1.5, 0.0, -22.95);
	pobst[120] = vec3(-1.35, 0.0, -22.95);
	pobst[121] = vec3(-1.2, 0.0, -22.95);
	pobst[122] = vec3(-1.05, 0.0, -22.95);
	pobst[123] = vec3(-0.9, 0.0, -22.95);
	pobst[124] = vec3(-0.75, 0.0, -22.95);
	pobst[125] = vec3(-0.6, 0.0, -22.95);
	pobst[126] = vec3(-0.45, 0.0, -22.95);
	pobst[127] = vec3(-0.3, 0.0, -22.95);
	pobst[128] = vec3(-0.15, 0.0, -22.95);
	pobst[129] = vec3(0.0, 0.0, -22.95);

	//(-)-----------xx
	pobst[130] = vec3(0.15, 0.0, -22.95);
	pobst[131] = vec3(0.3, 0.0, -22.95);
	pobst[132] = vec3(0.45, 0.0, -22.95);
	pobst[133] = vec3(0.6, 0.0, -22.95);
	pobst[134] = vec3(0.75, 0.0, -22.95);
	pobst[135] = vec3(0.9, 0.0, -22.95);
	pobst[136] = vec3(1.05, 0.0, -22.95);
	pobst[137] = vec3(1.2, 0.0, -22.95);
	pobst[138] = vec3(1.35, 0.0, -22.95);
	pobst[139] = vec3(1.65, 0.0, -22.95);

	pobst[140] = vec3(0.0, 0.0, -23.25);// |
	pobst[141] = vec3(0.0, 0.0, -23.55);// x
	pobst[142] = vec3(0.0, 0.0, -23.85);// |
	pobst[143] = vec3(0.0, 0.0, -24.15);
	pobst[144] = vec3(0.0, 0.0, -24.45);

	/* Cruz 2 */
	pobst[145] = vec3(1.5, 0.0, -25.2);// |
	pobst[146] = vec3(1.5, 0.0, -25.5);// x
	pobst[147] = vec3(1.5, 0.0, -25.8);// |
	pobst[148] = vec3(1.5, 0.0, -26.1);

	//---------xx--(x)
	pobst[149] = vec3(-1.95, 0.0, -26.4);
	pobst[150] = vec3(-1.8, 0.0, -26.4);
	pobst[151] = vec3(-1.65, 0.0, -26.4);
	pobst[152] = vec3(-1.5, 0.0, -26.4);
	pobst[153] = vec3(-1.35, 0.0, -26.4);
	pobst[154] = vec3(-1.2, 0.0, -26.4);
	pobst[155] = vec3(-1.05, 0.0, -26.4);
	pobst[156] = vec3(-0.9, 0.0, -26.4);
	pobst[157] = vec3(-0.75, 0.0, -26.4);
	pobst[158] = vec3(-0.3, 0.0, -26.4);
	pobst[159] = vec3(-0.15, 0.0, -26.4);

	//(x)--xxx--------
	pobst[160] = vec3(0.15, 0.0, -26.4);
	pobst[161] = vec3(0.3, 0.0, -26.4);
	pobst[162] = vec3(0.9, 0.0, -26.4);
	pobst[163] = vec3(1.05, 0.0, -26.4);
	pobst[164] = vec3(1.2, 0.0, -26.4);
	pobst[165] = vec3(1.35, 0.0, -26.4);
	pobst[166] = vec3(1.5, 0.0, -26.4);
	pobst[167] = vec3(1.65, 0.0, -26.4);
	pobst[168] = vec3(1.8, 0.0, -26.4);
	pobst[169] = vec3(1.95, 0.0, -26.4);

	pobst[170] = vec3(1.5, 0.0, -26.7);// |
	pobst[171] = vec3(1.5, 0.0, -27.0);// x
	pobst[172] = vec3(1.5, 0.0, -27.3);// |
	pobst[173] = vec3(1.5, 0.0, -27.6);
	pobst[174] = vec3(1.5, 0.0, -27.9);

	/* [Seccion 3] */

	/* H x 2 */
	pobst[175] = vec3(-0.9, 0.0, -31.95);
	pobst[176] = vec3(-0.9, 0.15, -31.95);
	pobst[177] = vec3(-0.6, 0.0, -31.95);
	pobst[178] = vec3(-0.6, 0.15, -31.95);

	pobst[179] = vec3(0.6, 0.0, -37.5);
	pobst[180] = vec3(0.6, 0.15, -37.5);
	pobst[181] = vec3(0.9, 0.0, -37.5);
	pobst[182] = vec3(0.9, 0.15, -37.5);

	/* Computacion Grafica (CG) awwwhhhh */
	pobst[183] = vec3(-0.6, 0.0, -41.55);
	pobst[184] = vec3(-0.45, 0.0, -41.55);
	pobst[185] = vec3(-0.3, 0.0, -41.55);

	pobst[186] = vec3(0.45, 0.0, -41.55);
	pobst[187] = vec3(0.6, 0.0, -41.55);

	pobst[188] = vec3(-0.75, 0.0, -41.7);

	pobst[189] = vec3(0.3, 0.0, -41.7);
	pobst[190] = vec3(0.6, 0.0, -41.7);
	pobst[191] = vec3(0.75, 0.0, -41.7);

	pobst[192] = vec3(-0.75, 0.0, -41.85);
	pobst[193] = vec3(0.3, 0.0, -41.85);

	pobst[194] = vec3(-0.6, 0.0, -42.0);
	pobst[195] = vec3(-0.45, 0.0, -42.0);
	pobst[196] = vec3(-0.3, 0.0, -42.0);
	pobst[197] = vec3(0.45, 0.0, -42.0);
	pobst[198] = vec3(0.6, 0.0, -42.0);

	/*Castillo */
	//primer y segundo piso piso
	pobst[199] = vec3(-1.95, 0.0, -44.55);
	pobst[200] = vec3(-1.95, 0.15, -44.55);

	pobst[201] = vec3(-1.8, 0.15, -44.55);

	pobst[202] = vec3(-1.65, 0.0, -44.55);
	pobst[203] = vec3(-1.65, 0.15, -44.55);

	pobst[204] = vec3(-1.5, 0.15, -44.55);

	pobst[205] = vec3(-1.35, 0.0, -44.55);
	pobst[206] = vec3(-1.35, 0.15, -44.55);

	pobst[207] = vec3(-1.2, 0.15, -44.55);

	pobst[208] = vec3(-1.05, 0.0, -44.55);
	pobst[209] = vec3(-1.05, 0.15, -44.55);

	pobst[210] = vec3(-0.9, 0.15, -44.55);

	pobst[211] = vec3(-0.75, 0.0, -44.55);
	pobst[212] = vec3(-0.75, 0.15, -44.55);

	pobst[213] = vec3(-0.6, 0.15, -44.55);

	pobst[214] = vec3(-0.45, 0.0, -44.55);
	pobst[215] = vec3(-0.45, 0.15, -44.55);

	pobst[216] = vec3(-0.3, 0.15, -44.55);

	pobst[217] = vec3(-0.15, 0.0, -44.55);
	pobst[218] = vec3(-0.15, 0.15, -44.55);

	pobst[219] = vec3(0.0, 0.15, -44.55);

	pobst[220] = vec3(0.15, 0.0, -44.55);
	pobst[221] = vec3(0.15, 0.15, -44.55);

	pobst[222] = vec3(0.3, 0.15, -44.55);

	pobst[223] = vec3(0.45, 0.0, -44.55);
	pobst[224] = vec3(0.45, 0.15, -44.55);

	pobst[225] = vec3(0.6, 0.15, -44.55);

	pobst[226] = vec3(0.75, 0.0, -44.55);
	pobst[227] = vec3(0.75, 0.15, -44.55);

	pobst[228] = vec3(0.9, 0.15, -44.55);

	pobst[229] = vec3(1.05, 0.0, -44.55);
	pobst[230] = vec3(1.05, 0.15, -44.55);

	pobst[231] = vec3(1.2, 0.15, -44.55);

	pobst[232] = vec3(1.35, 0.0, -44.55);
	pobst[233] = vec3(1.35, 0.15, -44.55);

	pobst[234] = vec3(1.5, 0.15, -44.55);

	pobst[235] = vec3(1.65, 0.0, -44.55);
	pobst[236] = vec3(1.65, 0.15, -44.55);

	pobst[237] = vec3(1.8, 0.15, -44.55);

	pobst[238] = vec3(1.95, 0.0, -44.55);
	pobst[239] = vec3(1.95, 0.15, -44.55);

	/* Carta Trampa */
	//x-xxx-xxx-xxx(-)xxx-xxx-xxx-x
	pobst[240] = vec3(-1.8, 0.0, -44.85);
	pobst[241] = vec3(-1.2, 0.0, -44.85);
	pobst[242] = vec3(-0.6, 0.0, -44.85);
	pobst[243] = vec3(0.0, 0.0, -44.85);
	pobst[244] = vec3(0.6, 0.0, -44.85);
	pobst[245] = vec3(1.2, 0.0, -44.85);
	pobst[246] = vec3(1.8, 0.0, -44.85);

	/* Estas siguientes posiciones se consideran para el renderizado pero no para evaluar casos de colision,
	ya que es imposible que exista por la altura en que estan ubicadas */
	//H
	pobst[247] = vec3(-0.9, 0.3, -31.95);
	pobst[248] = vec3(-0.6, 0.3, -31.95);
	pobst[249] = vec3(0.6, 0.3, -37.5);
	pobst[250] = vec3(0.9, 0.3, -37.5);

	//Castillo
	pobst[251] = vec3(-1.95, 0.3, -44.55);
	pobst[252] = vec3(-1.65, 0.3, -44.55);
	pobst[253] = vec3(-1.35, 0.3, -44.55);
	pobst[254] = vec3(-1.05, 0.3, -44.55);
	pobst[255] = vec3(-0.75, 0.3, -44.55);
	pobst[256] = vec3(-0.45, 0.3, -44.55);
	pobst[257] = vec3(-0.15, 0.3, -44.55);
	pobst[258] = vec3(0.15, 0.3, -44.55);
	pobst[259] = vec3(0.45, 0.3, -44.55);
	pobst[260] = vec3(0.75, 0.3, -44.55);
	pobst[261] = vec3(1.05, 0.3, -44.55);
	pobst[262] = vec3(1.35, 0.3, -44.55);
	pobst[263] = vec3(1.65, 0.3, -44.55);
	pobst[264] = vec3(1.95, 0.3, -44.55);
	return pobst;
}

/* Las monedas y los corazones estan ubicados dentro de un cubo de las mismas dimensiones que los obstaculos y el personaje
para que se vean centrados con respecto al juego, (ec. de posicion eje z:  n*-0.15 -0.075 + a/2)
- n es un entero cualquiera que no haga que el resultado de la ec se salga de los margenes del juego en el eje z (0 y-45)
- -0.075 es la mitad del ancho de un cubo (para que este centrado con respecto al cubo), y a es el ancho de un
corazon o una malla.*/

vec3* ubica_monedas(){
	vec3* pmon = (vec3 *)malloc(sizeof(vec3)*num_monedas); //0.15, 0.15, 0.022 dimensiones corazon
	//Ej: pmon[x] = vec3(0.0, 0.0, -0.214); // -0.214 = 1*(-0.15) -0.075 + 0.011

	/* [Seccion 1] */
	pmon[0] = vec3(-1.35, 0.0, -2.914);
	pmon[1] = vec3(1.35, 0.0, -2.914);

	pmon[2] = vec3(-0.45, 0.0, -3.814);
	pmon[3] = vec3(-0.3, 0.0, -3.814);
	pmon[4] = vec3(0.3, 0.0, -3.814);
	pmon[5] = vec3(0.45, 0.0, -3.814);

	pmon[6] = vec3(0.0, 0.0, -4.564);

	pmon[7] = vec3(0.0, 0.0, -8.464); //*3
	pmon[8] = vec3(0.0, 0.0, -8.764);
	pmon[9] = vec3(0.0, 0.0, -9.064);

	/* V */
	pmon[10] = vec3(-1.2, 0.0, -14.014);
	pmon[11] = vec3(1.2, 0.0, -14.014);

	pmon[12] = vec3(-1.35, 0.0, -14.164);
	pmon[13] = vec3(1.35, 0.0, -14.164);

	pmon[14] = vec3(-1.5, 0.0, -14.314);
	pmon[15] = vec3(1.5, 0.0, -14.314);

	/* [Seccion 2] */
	/* Flecha Doble */
	pmon[16] = vec3(-0.15, 0.0, -16.264); //*3
	pmon[17] = vec3(0.15, 0.0, -16.264);

	pmon[18] = vec3(0.0, 0.0, -16.714);

	pmon[19] = vec3(-0.15, 0.0, -17.164);
	pmon[20] = vec3(0.15, 0.0, -17.164);

	pmon[21] = vec3(-0.45, 0.0, -17.464);//<
	pmon[22] = vec3(0.45, 0.0, -17.464);//>

	pmon[23] = vec3(-0.3, 0.0, -17.614);
	pmon[24] = vec3(0.3, 0.0, -17.614);

	pmon[25] = vec3(-0.15, 0.0, -17.764);//<
	pmon[26] = vec3(0.15, 0.0, -17.764);//>

	pmon[27] = vec3(-0.45, 0.0, -17.914); //<
	pmon[28] = vec3(0.45, 0.0, -17.914);//>


	/* (Y) */
	pmon[29] = vec3(-1.65, 0.0, -21.064);
	pmon[30] = vec3(-1.5, 0.0, -21.064);

	pmon[31] = vec3(1.5, 0.0, -21.064);
	pmon[32] = vec3(1.65, 0.0, -21.064);

	pmon[33] = vec3(-1.65, 0.0, -21.514);
	pmon[34] = vec3(1.5, 0.0, -21.514);


	/* J of JUMP */
	pmon[35] = vec3(-0.3, 0.0, -27.964);
	pmon[36] = vec3(-0.14, 0.0, -27.964);
	pmon[37] = vec3(0.0, 0.0, -27.964);
	pmon[38] = vec3(0.0, 0.0, -28.264);
	pmon[39] = vec3(0.0, 0.0, -28.564);
	pmon[40] = vec3(0.0, 0.0, -28.864);

	/* [Seccion 3] */
	/* H */
	pmon[41] = vec3(-0.75, 0.15, -32.014);

	pmon[42] = vec3(0.75, 0.15, -37.564);

	/* Castillo */
	/* Entrada */
	pmon[43] = vec3(0.0, 0.0, -41.614);//*5
	pmon[44] = vec3(0.0, 0.0, -42.364);
	pmon[45] = vec3(0.0, 0.0, -43.114);
	pmon[46] = vec3(0.0, 0.0, -43.864);

	pmon[47] = vec3(-1.8, 0.0, -44.614);
	pmon[48] = vec3(-1.5, 0.0, -44.614);
	pmon[49] = vec3(-1.2, 0.0, -44.614);
	pmon[50] = vec3(-0.9, 0.0, -44.614);
	pmon[51] = vec3(-0.6, 0.0, -44.614);
	pmon[52] = vec3(-0.3, 0.0, -44.614);
	pmon[53] = vec3(0.0, 0.0, -44.614);//
	pmon[54] = vec3(0.3, 0.0, -44.614);
	pmon[55] = vec3(0.6, 0.0, -44.614);
	pmon[56] = vec3(0.9, 0.0, -44.614);
	pmon[57] = vec3(1.2, 0.0, -44.614);
	pmon[58] = vec3(1.5, 0.0, -44.614);
	pmon[59] = vec3(1.8, 0.0, -44.614);
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
	vec3* pcor = (vec3 *)malloc(sizeof(vec3)* num_corazones); //0.15, 0.15, 0.06 dimensiones corazon
	pcor[0] = vec3(1.5, 0.0, -22.995); //orig -22.95, -22.95 -0.075 + 0.03
	return pcor;
}

bool manejador_colisionesA(float px, float py, float pz, vec3 *pos_obs, int pvi, int pvf){
	//printf("px = %f\n", px);
	//printf("py = %f\n", py);
	//printf("pz = %f\n", pz);
	for(int i = pvi; i < pvf; i++){
		float auxx = fabs(pos_obs[i].v[0] - px);
		float auxy = fabs(pos_obs[i].v[1] - py);
		float auxz = fabs(pos_obs[i].v[2] - pz);
		//printf("auxx = %f\n",auxx);
		//printf("auxx = %f, auxy = %f, auxz = %f\n",auxx, auxy, auxz);
		if (auxz<0.15-0.01 && auxx <0.15-0.01 && auxy<0.15-0.01){ //-delta
			printf("ColisionA \n");
			return true;
		}
	}
	return false;
}

/*Estos metodos existen por el ajuste de las posiciones de las monedas y los corazones. Cuando hay una colision tipo A,
se estan comparando dos ubicaciones de dos objetos que son de las mismas dimensiones, si se aplicara lo mismo para una
moneda, la accion asociada a la colision se haria mas de una vez. Solo debe haber colision B o C cuando se toque al objeto
en si y no al cubo que lo contiene y hasta que deje de tocarse. Esto se soluciona con otro delimitador en el if para el
eje z que es donde se ubica la variacion.*/
bool manejador_colisionesB(float px, float py, float pz, vec3 *pos_mon, int pvi, int pvf){
	for(int i = pvi; i < pvf; i++){
		float auxx = fabs(pos_mon[i].v[0] - px);
		float auxy = fabs(pos_mon[i].v[1] - py);
		float auxz = fabs(pos_mon[i].v[2] - pz);
		if (auxz<0.15-0.01 && auxz>0.128 -0.01 && auxx <0.15-0.01 && auxy<0.15-0.01){
			printf("ColisionB \n");
			return true;
		}
	}
	return false;
}

bool manejador_colisionesC(float px, float py, float pz, vec3 *pos_cor, int pvi, int pvf){
	for(int i = pvi; i < pvf; i++){
		float auxx = fabs(pos_cor[i].v[0] - px);
		float auxy = fabs(pos_cor[i].v[1] - py);
		float auxz = fabs(pos_cor[i].v[2] - pz);
		if (auxz<0.09-0.05 && auxx <0.15 -0.01 && auxy<0.15-0.01){ //-0.05 magic number
			printf("ColisionC \n");
			return true;
		}
	}
	return false;
}
