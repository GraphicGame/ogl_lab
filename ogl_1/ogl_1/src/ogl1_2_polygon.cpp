#define __exec

#include <stdio.h>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <glut.h>

#include "common_header.h"

static const int WW = 960;
static const int WH = 640;
static float s_rot_angle = 0.0f;
static float s_z = -2.0f;

enum move_state {
	S_NEAR = 0,
	S_FAR
};
move_state s_state = S_FAR;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_SMOOTH);
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
	glTranslatef(-1.0f, 0, -2);

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(-0.6f, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(-0.3f, 0.5f, 0);
	glEnd();

	glTranslatef(2.8f, 0, -1.0f);

	glBegin(GL_QUADS);
	glVertex3f(-0.5f, 0.5f, 0);
	glVertex3f(-0.5f, -0.5f, 0);
	glVertex3f(0.5f, -0.5f, 0);
	glVertex3f(0.5f, 0.5f, 0);
	glEnd();

	glLoadIdentity();
	glTranslatef(0, -0.8f, s_z);
	if (s_state == S_FAR) {
		s_z -= 0.1f;
		if (s_z <= -20.0f) {
			s_state = S_NEAR;
		}
	}
	else if (s_state == S_NEAR) {
		s_z += 0.1f;
		if (s_z >= -2.0f) {
			s_state = S_FAR;
		}
	}

	glRotatef(s_rot_angle, 1, 1, 0);
	printf("s_rot_angle=%f", s_rot_angle);
	s_rot_angle += 1.0f;
	if (s_rot_angle > 360) {
		s_rot_angle = 0.0f;
	}
	float cube_points[8][3] = {
		{ -0.5f, 0.5f, -0.5f }, //0
		{ -0.5f, 0.5f, 0.5f },  //1
		{ 0.5f, 0.5f, 0.5f },   //2
		{ 0.5f, 0.5f, -0.5f },  //3
		{ -0.5f, -0.5f, -0.5f },//4
		{ -0.5f, -0.5f, 0.5f }, //5
		{ 0.5f, -0.5f, 0.5f },  //6
		{ 0.5f, -0.5f, -0.5f }, //7
	};
	glBegin(GL_TRIANGLES);
	glColor3f(1, 0, 0);
	glVertex3fv(cube_points[0]);
	glVertex3fv(cube_points[1]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[1]);
	glVertex3fv(cube_points[2]);
	glColor3f(0, 1, 0);
	glVertex3fv(cube_points[1]);
	glVertex3fv(cube_points[5]);
	glVertex3fv(cube_points[2]);
	glVertex3fv(cube_points[2]);
	glVertex3fv(cube_points[5]);
	glVertex3fv(cube_points[6]);
	glColor3f(0, 0, 1);
	glVertex3fv(cube_points[4]);
	glVertex3fv(cube_points[7]);
	glVertex3fv(cube_points[5]);
	glVertex3fv(cube_points[7]);
	glVertex3fv(cube_points[6]);
	glVertex3fv(cube_points[5]);
	glColor3f(1, 1, 0);
	glVertex3fv(cube_points[0]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[4]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[7]);
	glVertex3fv(cube_points[4]);
	glColor3f(0, 1, 1);
	glVertex3fv(cube_points[0]);
	glVertex3fv(cube_points[4]);
	glVertex3fv(cube_points[1]);
	glVertex3fv(cube_points[1]);
	glVertex3fv(cube_points[4]);
	glVertex3fv(cube_points[5]);
	glColor3f(1, 0.3f, 0.2f);
	glVertex3fv(cube_points[2]);
	glVertex3fv(cube_points[6]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[3]);
	glVertex3fv(cube_points[6]);
	glVertex3fv(cube_points[7]);
	glEnd();

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "2");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif