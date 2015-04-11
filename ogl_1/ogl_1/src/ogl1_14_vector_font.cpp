#define __exec

#include <stdio.h>
#include <windows.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

static HDC s_hdc;
static HFONT s_hfont;
static GLuint s_font_dislist;
static GLfloat s_angle = 0.0f;

static void init_gl() {
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

	glEnable(GL_COLOR_MATERIAL);

	float l_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f};
	float l_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float l_pos[] = { -10.0f, 10.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

static void build_font() {
	s_hfont = CreateFontW(
		23, 0,
		0, 0,
		FW_SEMIBOLD,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH,
		L"simhei"
		);
	s_font_dislist = glGenLists(1);
}

static void print_text_wchar_t(const wchar_t *text) {
	SelectObject(s_hdc, s_hfont);
	GLYPHMETRICSFLOAT gmf[1];
	glLoadIdentity();
	glColor3f(0.9, 0.9f, 0);
	glTranslatef(-3.0f, 0, -10.0f);
	glTranslatef(3.0f, 0, 0);
	glRotatef(s_angle, 0, 1, 0);
	glTranslatef(-3.0f, 0, 0);
	if (++s_angle > 360.0f) {
		s_angle = 0.0f;
	}

	for (int i = 0; text[i] != '\0'; i++) {
		wglUseFontOutlinesW(s_hdc, text[i], 1, s_font_dislist, 0.0f, 0.2f, WGL_FONT_POLYGONS, gmf);
		glNormal3f(0, 0, 1);
		glCallList(s_font_dislist);
	}
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	print_text_wchar_t(L"ÄãºÃOpenGL£¡");

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "14");
	s_hdc = GetDC(GetActiveWindow());

	init_gl();
	build_font();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif