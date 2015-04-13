//#define __exec

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
static GLfloat s_angle = 0.0f;

static GLuint s_floor_dislist = 0;
static GLuint s_sky_dislist = 0;
static bool s_fog_open = false;

typedef struct {
	GLfloat x, y, z;
	GLfloat rot_x, rot_y;
} simple_viewer;
static simple_viewer s_viewer;

static void init_fog() {
	float f_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, f_color);
	glFogf(GL_FOG_DENSITY, 0.002f);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 20.0f);
}

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	float l_ambient[] = { 2.5f, 2.5f, 2.5f, 1.0f };
	float l_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float l_pos[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	init_fog();
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	//glRotatef(s_viewer.rot_y, 0, -1, 0);
	glTranslatef(0, 2.0f, 0);
	glRotatef(20, 1, 0, 0);
	glRotatef(s_angle, 0, 1, 0);
	if (++s_angle > 360.0f) {
		s_angle = 0.0f;
	}
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[0]);
	glCallList(s_cube_dislist);

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	glRotatef(s_viewer.rot_x, -1, 0, 0);
	glRotatef(s_viewer.rot_y, 0, -1, 0);
	glCallList(s_floor_dislist);

	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[2]);
	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	glRotatef(s_viewer.rot_x, -1, 0, 0);
	glRotatef(s_viewer.rot_y, 0, -1, 0);
	glTranslatef(0, 0, 7);
	glCallList(s_sky_dislist);

	flush();
}

static void build_dislist() {
	float cube_vertexes[8][3] = {
		{ -1, 1, -1 },
		{ -1, 1, 1 },
		{ 1, 1, 1 },
		{ 1, 1, -1 },
		{ -1, -1, -1 },
		{ -1, -1, 1 },
		{ 1, -1, 1 },
		{ 1, -1, -1 },
	};

	s_cube_dislist = glGenLists(1);
	glNewList(s_cube_dislist, GL_COMPILE);
	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[1]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[2]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[3]);

	glNormal3f(0, 0, 1);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[1]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[5]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[2]);

	glNormal3f(0, -1, 0);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[4]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[5]);

	glNormal3f(0, 0, -1);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[3]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[4]);

	glNormal3f(-1, 0, 0);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[0]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[4]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[5]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[1]);

	glNormal3f(1, 0, 0);
	glTexCoord2d(0, -1); glVertex3fv(cube_vertexes[2]);
	glTexCoord2d(0, 0); glVertex3fv(cube_vertexes[6]);
	glTexCoord2d(1, 0); glVertex3fv(cube_vertexes[7]);
	glTexCoord2d(1, -1); glVertex3fv(cube_vertexes[3]);

	glEnd();
	glEndList();

	s_floor_dislist = glGenLists(1);
	glNewList(s_floor_dislist, GL_COMPILE);

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2d(0, -6); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2d(0, 0); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2d(6, 0); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2d(6, -6); glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	glEndList();

	s_sky_dislist = glGenLists(1);
	glNewList(s_sky_dislist, GL_COMPILE);

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2d(0, -1); glVertex3f(-100.0f, 100.0f, -100.0f);
	glTexCoord2d(0, 0); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2d(1, 0); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2d(1, -1); glVertex3f(100.0f, 100.0f, -100.0f);

	glTexCoord2d(0, -1); glVertex3f(-100.0f, 100.0f, 100.0f);
	glTexCoord2d(0, 0); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2d(-1, 0); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2d(-1, -1); glVertex3f(-100.0f, 100.0f, -100.0f);

	glTexCoord2d(0, -1); glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2d(0, 0); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2d(1, 0); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2d(1, -1); glVertex3f(100.0f, 100.0f, -100.0f);

	glTexCoord2d(0, -1); glVertex3f(-100.0f, 100.0f, 100.0f);
	glTexCoord2d(0, 0); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2d(-1, 0); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2d(-1, -1); glVertex3f(100.0f, 100.0f, 100.0f);

	glTexCoord2d(0, -1); glVertex3f(-100.0f, 100.0f, 100.0f);
	glTexCoord2d(0, 0); glVertex3f(-100.0f, 100.0f, -100.0f);
	glTexCoord2d(-1, 0); glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2d(-1, -1); glVertex3f(100.0f, 100.0f, 100.0f);

	glEnd();

	glEndList();
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/texture1.jpg", s_images[0]);

	s_images.push_back(new image_data());
	img_load_image("E:/floor.jpg", s_images[1]);

	s_images.push_back(new image_data());
	img_load_image("E:/sky.jpg", s_images[2]);

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

static void on_key(uchar code, int x, int y) {
	static GLfloat step = 0.1f;
	switch (code) {
	case 'f':
		s_fog_open = !s_fog_open;
		if (s_fog_open)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
		break;
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
	case 'q':
		s_viewer.y += step;
		break;
	case 'e':
		s_viewer.y -= step;
		break;
	case 'u':
		s_viewer.rot_y += 1.0f;
		break;
	case 'i':
		s_viewer.rot_y -= 1.0f;
		break;
	case 'j':
		s_viewer.rot_x += 1.0f;
		break;
	case 'k':
		s_viewer.rot_x -= 1.0f;
		break;
	default:
		break;
	}
}

static void init_viewer() {
	s_viewer.x = 0;
	s_viewer.y = 6.0f;
	s_viewer.z = 22.0f;
	s_viewer.rot_x = 0.0f;
	s_viewer.rot_y = 0.0f;
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "16");

	init_gl();
	load_gl_textures();
	build_dislist();
	init_viewer();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif