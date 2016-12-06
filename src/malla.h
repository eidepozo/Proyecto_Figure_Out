#ifndef MALLA_H
#define MALLA_H

using namespace std;

class malla{
    private:
        GLuint vao, vbo;
        int numvertices;
        vec3 pos;
        char* filename;

    public:
        mat4 M;
        malla(char *filename);
        malla();
        // gets
        GLuint getvao();
        int getnumvertices();
        vec3 getpos();
        char* getfilename();

        //sets
        void setvao(GLuint vao);
        void setnumvertices(int num);
        void setpos(vec3 p);
        void setfilename(char *f);
        
};

#endif
