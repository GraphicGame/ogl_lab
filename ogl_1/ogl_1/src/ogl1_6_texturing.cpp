#define __exec

#include <stdio.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

#include "image.h"

static float s_angle = 0.0f;
static image_data *s_image1 = nullptr;
static image_data *s_image2 = nullptr;
static GLuint s_texture1_id = 0;
static GLuint s_texture2_id = 0;

static int s_rot_x = 0;
static int s_rot_y = 0;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, -0.6f, -1.7f);
	glRotatef(s_angle, s_rot_x, s_rot_y, 0);
	if (s_rot_x != 0 || s_rot_y != 0) {
		s_angle += 1.0f;
		if (s_angle > 360.0f) {
			s_angle = 0.0f;
		}
	}
	
	float cube_vertices[8][3] = {
		{ -0.5f, 0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, -0.5f },
	};

	glBindTexture(GL_TEXTURE_2D, s_texture1_id);
	glShadeModel(GL_SMOOTH);
	
	glBegin(GL_QUADS);

	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[2]);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[3]);

	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[2]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[5]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[6]);

	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[4]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[7]);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[6]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[5]);

	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[3]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[7]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[4]);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, s_texture2_id);

	glBegin(GL_QUADS);

	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[4]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[5]);

	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[2]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[6]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[7]);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[3]);

	glEnd();

	flush();
}

static void load_image() {
	if (s_image1 == nullptr) {
		s_image1 = new image_data();
	}
	img_load_image("E:/texture1.jpg", s_image1);

	if (s_image2 == nullptr) {
		s_image2 = new image_data();
	}
	img_load_image("E:/texture2.jpg", s_image2);


	glGenTextures(1, &s_texture1_id);
	glBindTexture(GL_TEXTURE_2D, s_texture1_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_image1->width, s_image1->height, 0, s_image1->get_gl_format(), GL_UNSIGNED_BYTE, s_image1->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &s_texture2_id);
	glBindTexture(GL_TEXTURE_2D, s_texture2_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_image2->width, s_image2->height, 0, s_image2->get_gl_format(), GL_UNSIGNED_BYTE, s_image2->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete s_image1;
	s_image1 = nullptr;
	delete s_image2;
	s_image2 = nullptr;
}

static void on_key(uchar code, int x, int y) {
	switch (code) {
	case 'x':
		if (s_rot_x == 1)
			s_rot_x = 0;
		else
			s_rot_x = 1;
		break;
	case 'y':
		if (s_rot_y == 1)
			s_rot_y = 0;
		else
			s_rot_y = 1;
		break;
	default:
		break;
	}
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "6");

	init_gl();
	load_image();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif