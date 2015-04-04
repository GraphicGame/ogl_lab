//#define __exec
#include <stdio.h>

#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

#include "common_header.h"

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (GLfloat)WW / (GLfloat)WH, 0.1f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, -1);
	glColor3f(0, 1, 0);
	glVertex3f(1, 0, -1);
	glColor3f(0, 0, 1);
	glVertex3f(0.5, 0.5, -2);
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "1");

	init_gl();
	glutDisplayFunc(on_draw);

	glutMainLoop();

	return 0;
}
#endif