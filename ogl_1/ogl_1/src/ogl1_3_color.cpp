//#define __exec

#include <stdio.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

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

	glTranslatef(-1.0f, 0, -2.0f);
	float tri_points[3][3] = {
		{ -0.5f, 0.0f, 0.0f },
		{ 0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.5f, 0.0f}
	};
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3fv(tri_points[0]);
	glColor3f(0, 1, 0);
	glVertex3fv(tri_points[1]);
	glColor3f(0, 0, 1);
	glVertex3fv(tri_points[2]);
	glEnd();

	glLoadIdentity();
	glTranslatef(0.5f, 0, -2.0f);
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3fv(tri_points[0]);
	glColor3f(0, 1, 0);
	glVertex3fv(tri_points[1]);
	glColor3f(0, 0, 1);
	glVertex3fv(tri_points[2]);
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "3");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif