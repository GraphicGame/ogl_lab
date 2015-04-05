//#define __exec

#include <stdio.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

static float s_angle = 0.0f;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, -0.6f, -2.0f);
	glRotatef(s_angle, 0, 1, 0);
	s_angle += 1.0f;
	if (s_angle > 360) {
		s_angle = 0.0f;
	}

	float pyramid_points[5][3] = {
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f, 0.5f },
		{ 0.5f, 0.0f, 0.5f },
		{ 0.5f, 0.0f, -0.5f },
		{ 0.0f, 0.5f, 0.0f }
	};
	glBegin(GL_TRIANGLES);
	color_red;
	glVertex3fv(pyramid_points[4]);
	glVertex3fv(pyramid_points[1]);
	glVertex3fv(pyramid_points[2]);
	color_green;
	glVertex3fv(pyramid_points[4]);
	glVertex3fv(pyramid_points[0]);
	glVertex3fv(pyramid_points[1]);
	color_blue;
	glVertex3fv(pyramid_points[0]);
	glVertex3fv(pyramid_points[4]);
	glVertex3fv(pyramid_points[3]);
	color_yellow;
	glVertex3fv(pyramid_points[4]);
	glVertex3fv(pyramid_points[2]);
	glVertex3fv(pyramid_points[3]);
	
	glColor3f(1, 1, 1);
	glVertex3fv(pyramid_points[0]);
	glVertex3fv(pyramid_points[2]);
	glVertex3fv(pyramid_points[1]);

	glVertex3fv(pyramid_points[2]);
	glVertex3fv(pyramid_points[0]);
	glVertex3fv(pyramid_points[3]);
	
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "5");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif