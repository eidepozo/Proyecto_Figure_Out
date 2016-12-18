#ifndef MALLA_H
#define MALLA_H

#define TRIANGLE_FACTOR 3
#define QUAD_FACTOR     4

using namespace std;

class malla{
    private:
        char* filename;
        GLuint nvertices;
        GLuint nfaces;
        vec3 pos;
        GLuint FTYPE;
        GLuint FFACTOR;
        GLuint vao, vbo, nbo, tbo, ebo;
	      GLuint matloc;

    protected:
        GLuint tex;
        GLuint shaderprog;

    public:
        mat4 M;
        malla(const char *filename, GLuint shadprog, GLuint face_type);
        malla();

        // functions
        void render_vertices_points(GLfloat point_size);
        void render_vertices();
        void render_indices();
        void render_indices(mat4 matriz);
        bool load_mesh(const char* file_name, GLuint face_type);
        bool load_texture(const char* file_name);
        void makevao(GLfloat *vertices, GLfloat* normals, GLfloat* texcoords, GLuint *indices);
        void mtranslate(vec3 d);
        void mrotatey(float d);
        void reset_matrix();
        void print_matrix();

        // gets
        vec3 get_pos();
        mat4 get_matrix();
        GLuint get_vao();
        GLuint get_facetype();
        GLuint get_facefactor();
        GLuint get_matloc();
        char* get_filename();
        int get_numvertices();
        int get_numfaces();


        // sets
        void set_pos(vec3 p);
        void set_facetype(GLuint FTYPE);
        void set_facefactor(GLuint FFACTOR);
        void set_numvertices(int nvert);
        void set_numfaces(int ntri);
        void set_matrix(mat4 M);
        void set_matloc(GLuint matloc);
        void set_shaderprog(GLuint sprog);

};

#endif
