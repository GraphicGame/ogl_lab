//#define __exec

#include <stdio.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

static float s_tri_angle = 0.0f;
static float s_quad_angle = 0.0f;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)WW / (GLfloat)WH, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float tri_points[3][3] = {
		{ -0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f},
		{ 0.0f, 0.5f, 0.0f }
	};

	glTranslatef(-0.8f, 0.0f, -2.0f);
	glRotatef(s_tri_angle, 0, 0, 1);
	s_tri_angle += 1.0f;
	if (s_tri_angle > 360) {
		s_tri_angle = 0.0f;
	}
	glBegin(GL_TRIANGLES);
	color_red; glVertex3fv(tri_points[0]);
	color_green; glVertex3fv(tri_points[1]);
	color_blue; glVertex3fv(tri_points[2]);
	glEnd();

	float quad_points[4][3] = {
		{ -0.5f, 0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{ 0.5f, -0.5f, 0.0f },
		{ 0.5f, 0.5f, 0.0f }
	};

	glLoadIdentity();
	glTranslatef(0.8f, 0.0f, -3.0f);
	glRotatef(s_quad_angle, 0, 0, -1);
	s_quad_angle += 1.0f;
	if (s_quad_angle > 360) {
		s_quad_angle = 0.0f;
	}
	glBegin(GL_QUADS);
	color_red; glVertex3fv(quad_points[0]);
	color_green; glVertex3fv(quad_points[1]);
	color_blue; glVertex3fv(quad_points[2]);
	color_yellow; glVertex3fv(quad_points[3]);
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "4");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif