#define __exec

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

static GLuint s_cube_dislist = 0;

static enum quadric_type {
	QUADRIC_CUBE = 0,
	QUADRIC_SPHERE,
	QUADRIC_CONE,
	QUADRIC_CYLINDER
};
static quadric_type s_quadric_type = QUADRIC_CUBE;

typedef struct {
	GLfloat x, y, z;
} simple_viewer;

static simple_viewer s_viewer;
static GLUquadric *s_GLUquadric_obj;
static GLfloat s_rot_angle = 0.0f;

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
	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	glTranslatef(0, 0, -6.0f);
	glRotatef(s_rot_angle, 0, 1, 0);
	if (++s_rot_angle > 360.0f) {
		s_rot_angle = 0.0f;
	}

	switch (s_quadric_type) {
	case QUADRIC_CUBE:
		glCallList(s_cube_dislist);
		break;
	case QUADRIC_SPHERE:
		gluSphere(s_GLUquadric_obj, 1.0f, 100, 100);
		break;
	case QUADRIC_CONE:
		gluCylinder(s_GLUquadric_obj, 1.0f, 0.0f, 2.0f, 100, 100);
		break;
	case QUADRIC_CYLINDER:
		gluCylinder(s_GLUquadric_obj, 1.0f, 0.6f, 2.0f, 100, 100);
		break;
	default:
		break;
	}

	flush();
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/texture1.jpg", s_images[0]);
	s_images.push_back(new image_data());
	img_load_image("E:/texture2.jpg", s_images[1]);

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < s_images.size(); i++) {
		image_data *img = s_images[i];
		s_tex_ids.push_back(0);
		glGenTextures(1, &s_tex_ids[i]);
		glBindTexture(GL_TEXTURE_2D, s_tex_ids[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, img->get_gl_format(), GL_UNSIGNED_BYTE, img->pixels);
	}
}

static void build_list() {
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

static void init_viewer() {
	s_viewer.x = 0;
	s_viewer.y = 0;
	s_viewer.z = 4;
}

static void on_key(uchar code, int x, int y) {
	float step = 0.1f;
	switch (code) {
	case 'w':
		s_viewer.z -= step;
		break;
	case 'a':
		s_viewer.x -= step;
		break;
	case 's':
		s_viewer.z += step;
		break;
	case 'd':
		s_viewer.x += step;
		break;
	case ' ':
		s_quadric_type = (enum quadric_type)((int)s_quadric_type + 1);
		if (s_quadric_type > QUADRIC_CYLINDER) {
			s_quadric_type = QUADRIC_CUBE;
		}
		break;
	default:
		break;
	}
}

static void init_quadrics() {
	s_GLUquadric_obj = gluNewQuadric();
	gluQuadricNormals(s_GLUquadric_obj, GL_SMOOTH);
	gluQuadricTexture(s_GLUquadric_obj, GL_TRUE);
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "17");

	init_gl();
	load_gl_textures();
	build_list();
	init_viewer();
	init_quadrics();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif