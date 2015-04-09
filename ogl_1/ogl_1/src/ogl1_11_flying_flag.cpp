#define __exec

#include <stdio.h>
#include <vector>
using namespace std;

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "image.h"

static vector<image_data*> s_images;
static vector<GLuint> s_tex_ids;

static void init_gl() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();



	flush();
}

static void load_gl_textures() {
	
}

static void init_flag_model() {
	
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "11");

	init_gl();
	load_gl_textures();
	init_flag_model();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif