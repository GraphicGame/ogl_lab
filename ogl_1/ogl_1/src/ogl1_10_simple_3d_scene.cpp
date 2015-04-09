//#define __exec

#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "image.h"

typedef struct {
	GLfloat x, y, z;
	GLfloat u, v;
} vertex;

typedef struct {
	vertex v[4];
} polygon;

static vector<image_data*> s_images;
static vector<GLuint> s_tex_ids;
static vector<polygon*> s_polygons;

typedef struct {
	GLfloat x, y, z;
	GLfloat rot_y;
} simple_viewer;
static simple_viewer s_viewer;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect_ratio, 0.1f, 600.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	GLfloat l_ambient[] = { 2.5f, 2.5f, 2.5f, 1.0f };
	GLfloat l_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l_pos[] = { 0.0f, 2.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, s_tex_ids[0]);

	glTranslatef(-s_viewer.x, -s_viewer.y, -s_viewer.z);
	glRotatef(-s_viewer.rot_y, 0, 1, 0);
	for (int i = 0; i < s_polygons.size(); i++) {
		polygon *poly = s_polygons[i];
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		for (int vi = 0; vi < 4; vi++) {
			glTexCoord2f(poly->v[vi].u, poly->v[vi].v);
			glVertex3f(poly->v[vi].x, poly->v[vi].y, poly->v[vi].z);
		}
		glEnd();
	}

	flush();
}

static GLfloat s_speed = 0.1f;
static void on_key(uchar code, int x, int y) {
	switch (code) {
	case 'w':
		s_viewer.z -= s_speed;
		break;
	case 'a':
		s_viewer.x -= s_speed;
		break;
	case 's':
		s_viewer.z += s_speed;
		break;
	case 'd':
		s_viewer.x += s_speed;
		break;
	case 'q':
		s_viewer.y += s_speed;
		break;
	case 'e':
		s_viewer.y -= s_speed;
		break;
	case 'j':
		s_speed -= 0.1f;
		break;
	case 'k':
		s_speed += 0.1f;
		break;
	case 'u':
		s_viewer.rot_y += 0.6f;
		break;
	case 'i':
		s_viewer.rot_y -= 0.6f;
		break;
	default:
		break;
	}
}

static void load_gl_textures() {
	s_images.push_back(new image_data());
	img_load_image("E:/floor.jpg", s_images[0]);

	for (uint i = 0; i < s_images.size(); i++) {
		image_data *img = s_images[i];
		s_tex_ids.push_back(0);
		glGenTextures(1, &s_tex_ids[i]);
		glBindTexture(GL_TEXTURE_2D, s_tex_ids[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, img->get_gl_format(), GL_UNSIGNED_BYTE, img->pixels);

		delete img;
	}
	glEnable(GL_TEXTURE_2D);
}

static bool is_ignore_line(const string &line) {
	for (int i = 0; i < line.size(); i++) {
		char c = line[i];
		if (c != ' ' && c != '\t' && c != '\n') {
			if (c == '#')
				return true;
			return false;
		}
	}
	return true;
}

static void load_scene_file(const string &file_name, vector<string> &lines) {
	ifstream file(file_name);
	while (!file.eof()) {
		string l;
		getline(file, l);
		if (!is_ignore_line(l)) {
			lines.push_back(l);
		}
	}
	file.close();
}

static void read_tokens(const string &line, vector<string> &tokens) {
	tokens.clear();
	bool inword = false;
	string token;
	for (int i = 0; i < line.size(); i++) {
		char c = line[i];
		if (c == ' ' || c == '\n' || c == '\t') {
			if (inword) {
				tokens.push_back(token);
				token.clear();
				inword = false;
			}
		}
		else {
			inword = true;
			token += c;
		}
	}
	if (token.size() > 0) {
		tokens.push_back(token);
	}
}

static void load_scene() {
	vector<string> lines;
	vector<string> tokens;
	load_scene_file("E:/scene.txt", lines);

	read_tokens(lines[0], tokens);
	int quads_count = atoi(tokens[1].c_str());
	for (int i = 0; i < quads_count; i++) {
		polygon *poly = new polygon();
		for (int vi = 0; vi < 4; vi++) {
			read_tokens(lines[vi + 1 + i * 4], tokens);
			GLfloat x = atof(tokens[0].c_str());
			GLfloat y = atof(tokens[1].c_str());
			GLfloat z = atof(tokens[2].c_str());
			GLfloat u = atof(tokens[3].c_str());
			GLfloat v = atof(tokens[4].c_str());
			poly->v[vi] = { x, y, z, u, v };
		}
		s_polygons.push_back(poly);
	}
}

static void init_viewer() {
	s_viewer.x = 0;
	s_viewer.y = 2;
	s_viewer.z = 8;
	s_viewer.rot_y = 0;
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "10");

	init_gl();
	load_gl_textures();
	load_scene();
	init_viewer();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutKeyboardFunc(on_key);
	glutMainLoop();

	return 0;
}
#endif