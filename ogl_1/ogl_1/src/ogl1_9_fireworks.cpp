#define __exec

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
using namespace std;

#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "common_header.h"
#include "image.h"

static image_data *s_star = nullptr;
static GLuint s_tex_id = 0;

typedef struct {
	GLfloat x, y, z;
} vertex;

typedef struct {
	GLubyte r, g, b;
	vertex v1, v2, v3, v4;
	GLfloat tx, ty, tz;
	GLfloat down_angle;
	GLfloat down_speed;
	GLfloat down_rotate_speed;
} fw_node;

static const int num_nodes = 300;
static vector<fw_node*> s_fireworks;

enum fw_state {
	S_NULL = 0,
	S_UP,
	S_IDLE,
	S_EXPLOSION
};
fw_state s_state = S_NULL;

static GLfloat s_up_y = -6.0f;
static GLfloat s_up_speed = 1.0f;
static GLfloat s_up_angle = 0.0f;
static uint s_up_end_ts = 0.0f;

static void init_fireworks() {
	srand(time(NULL));
	for (int i = 0; i < num_nodes; i++) {
		fw_node *node = new fw_node();
		node->r = rand() & 0xff;
		node->g = rand() & 0xff;
		node->b = rand() & 0xff;
		node->tx = node->ty = node->tz = 0.0f;
		node->v1 = { -1, 1, 0 };
		node->v2 = { -1, -1, 0 };
		node->v3 = { 1, -1, 0 };
		node->v4 = { 1, 1, 0 };
		node->down_angle = rand() % 200;
		node->down_speed = (float)((rand() % 3) + 3) / 10.0f;
		node->down_rotate_speed = rand() % 6 + 2;
		s_fireworks.push_back(node);
	}

	s_fireworks[0]->r = 255;
	s_fireworks[0]->g = 255;
	s_fireworks[0]->b = 0;
}

static void reset_fireworks() {
	s_state = S_NULL;

	s_up_y = -6.0f;
	s_up_speed = 1.0f;
	s_up_angle = 0.0f;
	s_up_end_ts = 0.0f;

	for (int i = 0; i < s_fireworks.size(); i++) {
		delete s_fireworks[i];
		s_fireworks[i] = nullptr;
	}
	s_fireworks.clear();
	init_fireworks();
}

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float l_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float l_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float l_pos[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	init_fireworks();
}

static void assign_fireworks_position() {
	for (auto it = s_fireworks.begin(); it != s_fireworks.end(); it++) {
		fw_node *node = *it;
		node->tx = rand() % 16;
		node->ty = rand() % 6 + 8;
		node->tz = rand() % 5;
		if (rand() % 2 == 1) {
			node->tx = 0 - node->tx;
			node->tz = 0 - node->tz;
		}
	}
}

static void handle_state_up() {
	glLoadIdentity();
	glTranslatef(0, s_up_y, -10.0f);
	glRotatef(s_up_angle, 0, 0, 1);

	fw_node *node = s_fireworks[0];

	glColor4ub(node->r, node->g, node->b, 255);
	glBegin(GL_QUADS);
	glTexCoord2d(0, -1);
	glVertex3f(node->v1.x, node->v1.y, node->v1.z);
	glTexCoord2d(0, 0);
	glVertex3f(node->v2.x, node->v2.y, node->v2.z);
	glTexCoord2d(1, 0);
	glVertex3f(node->v3.x, node->v3.y, node->v3.z);
	glTexCoord2d(1, -1);
	glVertex3f(node->v4.x, node->v4.y, node->v4.z);
	glEnd();

	s_up_angle += 8.0f;
	if (s_up_angle > 360.0f) {
		s_up_angle = 0.0f;
	}

	s_up_y += s_up_speed;
	s_up_speed *= 0.91f;
	if (s_up_y >= 5.0f) {
		s_state = S_IDLE;
		assign_fireworks_position();
		glClearColor(0.7, 0.7, 0.7, 1);
		s_up_end_ts = time(NULL);
	}
}

static void handle_state_explosion() {
	for (auto it = s_fireworks.begin(); it != s_fireworks.end(); it++) {
		glLoadIdentity();

		fw_node *node = *it;
		glTranslatef(node->tx, node->ty - node->down_speed, -12.0f + node->tz);
		glRotatef(node->down_angle, 0, 0, 1);

		glColor4ub(node->r, node->g, node->b, 255);
		glBegin(GL_QUADS);
		glTexCoord2d(0, -1);
		glVertex3f(node->v1.x, node->v1.y, node->v1.z);
		glTexCoord2d(0, 0);
		glVertex3f(node->v2.x, node->v2.y, node->v2.z);
		glTexCoord2d(1, 0);
		glVertex3f(node->v3.x, node->v3.y, node->v3.z);
		glTexCoord2d(1, -1);
		glVertex3f(node->v4.x, node->v4.y, node->v4.z);
		glEnd();

		node->down_speed *= 1.05f;
		node->down_angle += node->down_rotate_speed;
		if (node->down_angle > 360.0f) {
			node->down_angle = 0.0f;
		}
	}
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, s_tex_id);

	switch (s_state) {
	case S_UP:
		handle_state_up();
		break;
	case S_EXPLOSION:
		handle_state_explosion();
		break;
	case S_IDLE:
		if (time(NULL) - s_up_end_ts >= 0.0f) {
			glClearColor(0, 0, 0, 1);
			s_state = S_EXPLOSION;
		}
		break;
	default:
		break;
	}

	flush();
}

static void on_key(uchar code, int x, int y) {
	switch (code) {
	case 'j':
		reset_fireworks();
		s_state = S_UP;
		break;
	default:
		break;
	}
}

static void load_gl_textures() {
	if (s_star == nullptr) {
		s_star = new image_data();
	}
	img_load_image("E:/star.jpg", s_star);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &s_tex_id);
	glBindTexture(GL_TEXTURE_2D, s_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_star->width, s_star->height, 0, s_star->get_gl_format(), GL_UNSIGNED_BYTE, s_star->pixels);
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "9");

	init_gl();
	load_gl_textures();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif