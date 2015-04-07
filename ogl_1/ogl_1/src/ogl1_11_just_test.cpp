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
	gluPerspective(90, aspect_ratio, 0.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void on_draw() {
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	glTranslatef(0, 0, -2.0f);
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-2.5, 0, 0);
	glVertex3f(0, 2.5, -2);
	glVertex3f(2.5, 0, 0);
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "test");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif