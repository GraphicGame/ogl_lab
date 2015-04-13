#define __exec

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "image.h"

static vector<image_data*> s_images;
static vector<GLuint> s_tex_ids;

typedef struct {
	GLfloat x, y, z;
} simple_viewer;

static simple_viewer s_viewer;

typedef struct {
	GLfloat life, fade;
	GLfloat r, g, b;
	GLfloat x, y, z;
	GLfloat xs, ys, zs; //speed.
	GLfloat xg, yg, zg; //gravity.
} particle;
static GLfloat s_base_x_speed = 0.0f;
static GLfloat s_base_y_speed = 0.0f;
static const int MAX_PARTICLE = 2000;
static vector<particle*> s_particles;

static void init_gl() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
}

static void reset_particle(particle *p) {
	p->life = 1.0f;
	p->fade = float(rand() % 100) / 1000.0f + 0.004f;
	p->r = rand() & 0xff;
	p->g = rand() & 0xff;
	p->b = rand() & 0xff;
	p->x = p->y = p->z = 0.0f;
	p->xs = s_base_x_speed + (float)(rand() % 20 - 10) / 40.0f + 0.08f;
	p->ys = s_base_y_speed + (float)(rand() % 20 - 10) / 40.0f + 0.08f;
	p->zs = (float)(rand() % 20 - 10) / 40.0f + 0.08f;
	p->xg = 0.0f;
	p->yg = -0.1f;
	p->zg = 0.0f;
}

static void init_particles() {
	srand(time(NULL));
	for (int i = 0; i < MAX_PARTICLE; i++) {
		particle *p = new particle();
		reset_particle(p);
		s_particles.push_back(p);
	}
}

static void draw_particles() {
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[0]);
	int sz = s_particles.size();
	for (int i = 0; i < sz; i++) {
		particle *p = s_particles[i];
		float px = p->x;
		float py = p->y;
		float pz = p->z;
		glColor4ub(p->r, p->g, p->b, p->life * 255);
		glBegin(GL_QUADS);
		glTexCoord2d(0, -1); glVertex3f(px - 0.2f, py + 0.2f, pz);
		glTexCoord2d(0,  0); glVertex3f(px - 0.2f, py - 0.2f, pz);
		glTexCoord2d(1,  0); glVertex3f(px + 0.2f, py - 0.2f, pz);
		glTexCoord2d(1, -1); glVertex3f(px + 0.2f, py + 0.2f, pz);
		glEnd();
		p->x += p->xs;
		p->y += p->ys;
		p->z += p->zs;
		p->xs += p->xg;
		p->ys += p->yg;
		p->zs += p->zg;
		p->life -= p->fade;
		if (p->life <= 0.0f) {
			reset_particle(p);
		}
	}
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	draw_particles();
	flush();
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/star.jpg", s_images[0]);
	s_images.push_back(new image_data());
	img_load_image("E:/peach_flower.jpg", s_images[1]);

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

static void init_viewer() {
	s_viewer.x = 0;
	s_viewer.y = 0;
	s_viewer.z = 10;
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
	case 'j':
		s_base_x_speed -= 0.005;
		break;
	case 'k':
		s_base_x_speed += 0.005;
		break;
	case 'o':
		s_base_y_speed += 0.005;
		break;
	case 'l':
		s_base_y_speed -= 0.005;
		break;
	default:
		break;
	}
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "17");

	init_gl();
	load_gl_textures();
	init_viewer();
	init_particles();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif