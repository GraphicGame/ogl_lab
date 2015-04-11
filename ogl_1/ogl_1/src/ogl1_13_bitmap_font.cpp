//#define __exec

#include <stdio.h>
#include <windows.h>
#include <vector>
using namespace std;
#include <time.h>

#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"
#include "utils.h"

static GLuint s_font_dislist = 0;
static HWND s_hwnd;
static HDC s_hdc;
static HFONT s_hfont;
static vector<wchar_t*> s_story;
static int s_line_index = 0;
static int s_line_word_index = 0;
static clock_t s_ts = 0;
static bool s_playing = true;

static void init_gl() {
	glClearColor(0, 0, 0, 1);
	glViewport(0, 0, WW, WH);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, aspect_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}

static void build_font() {
	s_hfont = CreateFontW(
		28, 0,
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

static void build_story() {
	s_story.push_back(L"某空姐执勤时发现一黑衣神秘男子意图打飞机！！打飞机！！！");
	s_story.push_back(L"由于事发突然，为了挽救机上乘客的生命财产安全，");
	s_story.push_back(L"该空姐在千钧一发的生死时刻，毅然决然的挺身而出，");
	s_story.push_back(L"用自己年轻的身体（嘴巴）堵住了枪口！！");
	s_story.push_back(L"这种舍己为人的大无畏精神，堪称新时代的黄继光！！！");
}

static void print_text(const char* text) {
	SelectObject(s_hdc, s_hfont);
	int len = 0;
	for (int i = 0; text[i] != '\0'; i++) {
		if (IsDBCSLeadByte(text[i]))
			++i;
		++len;
	}
	wchar_t *wtext = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, text, -1, wtext, len);
	wtext[len] = L'\0';
	for (int i = 0; i < len; i++) {
		wglUseFontBitmapsW(s_hdc, wtext[i], 1, s_font_dislist);
		glCallList(s_font_dislist);
	}
	free(wtext);
}

static void print_text_wchar_t(const wchar_t* text) {
	SelectObject(s_hdc, s_hfont);
	for (int i = 0; text[i] != '\0'; i++) {
		wglUseFontBitmapsW(s_hdc, text[i], 1, s_font_dislist);
		glCallList(s_font_dislist);
	}
}

static void do_story() {
	GLfloat line_x = -1.3f;
	GLfloat line_y = 0.8f;

	glColor3f(0.8, 0.8, 0);
	for (int i = 0; i < s_line_index; i++) {
		glRasterPos2f(line_x, line_y);
		print_text_wchar_t(s_story[i]);
		line_y -= 0.15f;
	}

	if (s_playing) {
		wchar_t *line = s_story[s_line_index];
		int w_index = s_line_word_index;
		wchar_t *words = (wchar_t*)malloc(sizeof(wchar_t)* (w_index + 2));
		for (int i = 0; i <= w_index; i++) {
			words[i] = line[i];
		}
		words[w_index + 1] = L'\0';

		glRasterPos2f(line_x, line_y);
		print_text_wchar_t(words);
		free(words);
	}
}

static void on_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -1.0f);
	
	do_story();
	clock_t curr_ts = clock();
	if (curr_ts - s_ts > 200 && s_playing) {
		s_ts = curr_ts;
		s_line_word_index++;
		wchar_t *line = s_story[s_line_index];
		int line_len = 0;
		while ((*line++) != L'\0') {
			++line_len;
		}
		if (s_line_word_index >= line_len) {
			s_line_word_index = 0;
			s_line_index++;
			if (s_line_index >= s_story.size()) {
				s_playing = false;
			}
		}
	}

	flush();
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "13");
	s_hwnd = GetActiveWindow();
	s_hdc = GetDC(s_hwnd);

	init_gl();
	build_font();
	build_story();

	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif