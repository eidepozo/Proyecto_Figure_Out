#ifndef TOOLS_H
#define TOOLS_H

bool load_mesh (const char* file_name, GLuint* vao, int* point_count);
bool gameplay(float cam_speed, double elapsed_seconds, float *cam_pos, float *cam_yaw, float cam_yaw_speed);
void init(int width, int height);

#endif
