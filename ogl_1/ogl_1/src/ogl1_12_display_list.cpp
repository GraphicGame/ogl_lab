//#define __exec

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
static GLuint s_cube_dislist;

static void init_gl() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[0]);

	glTranslatef(0, 0, -3.0f);
	glRotatef(30, 0, 1, 0);
	glCallList(s_cube_dislist);

	flush();
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/texture1.jpg", s_images[0]);

	for (uint i = 0; i < s_images.size(); i++) {
		image_data *img = s_images[i];
		s_tex_ids.push_back(0);
		glGenTextures(1, &s_tex_ids[i]);
		glBindTexture(GL_TEXTURE_2D, s_tex_ids[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, img->get_gl_format(), GL_UNSIGNED_BYTE, img->pixels);
	}
}

static void build_dislist() {
	GLfloat cube_vertexes[8][3] = {
		{ -1, 1, -1 },
		{ -1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, 1 },
		{ 1, -1, 1 },
		{ 1, -1, -1 }
	};

	s_cube_dislist = glGenLists(1);
	glNewList(s_cube_dislist, GL_COMPILE);
	glBegin(GL_QUADS);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[1]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[2]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[3]);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[1]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[5]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[2]);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[4]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[5]);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[3]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[4]);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[4]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[5]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[1]);

	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[2]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[3]);

	glEnd();
	glEndList();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "12");
	
	init_gl();
	load_gl_textures();
	build_dislist();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif