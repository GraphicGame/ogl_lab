#define __exec

#include <stdio.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "image.h"

static image_data *s_image = nullptr;

static GLfloat s_angle = 0.0f;
static GLfloat s_rot_x = 0;
static GLfloat s_rot_y = 0;

static GLuint s_tex_id = 0;
static bool s_light_on = false;

static GLfloat s_x = 0.0f;
static GLfloat s_z = -2.0f;

static void init_light() {
	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light_diffuse[] = { 2.0f, 2.0f, 2.0f, 1.0f };
	GLfloat light_pos[] = { 0.0f, -0.6f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_LIGHT0);
}

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

	GLfloat cube_vertices[8][3] = {
		{ -0.5f, 0.5f, -0.5f },
		{ -0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ 0.5f, 0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, -0.5f },
	};

	glTranslatef(s_x, -0.6f, s_z);
	glRotatef(s_angle, s_rot_x, s_rot_y, 0);
	if (s_rot_x != 0 || s_rot_y != 0) {
		s_angle += 1.0f;
		if (s_angle > 360.0f)
			s_angle = 0.0f;
	}

	glBindTexture(GL_TEXTURE_2D, s_tex_id);
	glShadeModel(GL_SMOOTH);

	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[2]);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[3]);

	glNormal3f(0, 0, 1);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[2]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[5]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[6]);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[4]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[7]);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[6]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[5]);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[3]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[7]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[4]);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(1, -1);
	glVertex3fv(cube_vertices[1]);
	glTexCoord2f(0, -1);
	glVertex3fv(cube_vertices[0]);
	glTexCoord2f(0, 0);
	glVertex3fv(cube_vertices[4]);
	glTexCoord2f(1, 0);
	glVertex3fv(cube_vertices[5]);

	glNormal3f(1, 0, 0);
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

static void on_key(uchar code, int x, int y) {
	switch (code) {
	case 'x':
		if (s_rot_x == 0)
			s_rot_x = 1;
		else
			s_rot_x = 0;
		break;
	case 'y':
		if (s_rot_y == 0)
			s_rot_y = 1;
		else
			s_rot_y = 0;
		break;
	case 'l':
		s_light_on = !s_light_on;
		if (s_light_on)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		break;
	case 'w':
		s_z += -0.06f;
		break;
	case 'a':
		s_x += -0.06f;
		break;
	case 's':
		s_z += 0.06;
		break;
	case 'd':
		s_x += 0.06f;
		break;
	default:
		break;
	}
}

static void load_image() {
	if (s_image == nullptr) {
		s_image = new image_data();
	}
	img_load_image("E:/texture3.jpg", s_image);

	glGenTextures(1, &s_tex_id);
	glBindTexture(GL_TEXTURE_2D, s_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_image->width, s_image->height, 0, s_image->get_gl_format(), GL_UNSIGNED_BYTE, s_image->pixels);
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "7");

	init_gl();
	load_image();
	init_light();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif