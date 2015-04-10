//#define __exec

#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "image.h"
#include "utils.h"

static vector<image_data*> s_images;
static vector<GLuint> s_tex_ids;

typedef struct {
	GLfloat x, y, z;
	GLfloat u, v;
} vertex;

static const int ROW = 50;
static const int COL = 50;
static vertex s_model_vertexes[ROW][COL];

static bool s_wave = true;

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

static void do_wave() {
	if (s_wave) {
		for (int row = 0; row < ROW; row++) {
			GLfloat first_z = s_model_vertexes[row][0].z;
			for (int col = 0; col < COL - 4; col++) {
				s_model_vertexes[row][col].z = s_model_vertexes[row][col + 1].z;
			}
			s_model_vertexes[row][COL - 4].z = first_z;
		}
	}
	//s_wave = !s_wave;
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[0]);

	glTranslatef(0, 0, -20.0f);
	glBegin(GL_QUADS);
	for (int row = 0; row < ROW - 1; row++) {
		for (int col = 0; col < COL - 1; col++) {
			vertex *v1 = &(s_model_vertexes[row][col]);
			vertex *v2 = &(s_model_vertexes[row + 1][col]);
			vertex *v3 = &(s_model_vertexes[row + 1][col + 1]);
			vertex *v4 = &(s_model_vertexes[row][col + 1]);
			
			glTexCoord2d(v1->u, v1->v);
			glVertex3f(v1->x, v1->y, v1->z);
			glTexCoord2d(v2->u, v2->v);
			glVertex3f(v2->x, v2->y, v2->z);
			glTexCoord2d(v3->u, v3->v);
			glVertex3f(v3->x, v3->y, v3->z);
			glTexCoord2d(v4->u, v4->v);
			glVertex3f(v4->x, v4->y, v4->z);
		}
	}
	glEnd();

	do_wave();

	flush();
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/flag.jpg", s_images[0]);

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

static void init_flag_model() {
	for (int row = 0; row < ROW; row++) {
		for (int col = 0; col < COL; col++) {
			GLfloat x = (col / 5.0f) - 5.0f;
			GLfloat y = (row / 5.0f) - 5.0f;
			GLfloat z = sin(deg_2_rad(col / 5.0f * 80));
			GLfloat u = (float)col / (COL - 1.0f);
			GLfloat v = (float)row / (ROW - 1.0f);
			s_model_vertexes[row][col] = {
				x, y, z, u, -v
			};
		}
	}
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