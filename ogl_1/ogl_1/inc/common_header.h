#ifndef common_header_h
#define common_header_h

#include <stdio.h>

typedef unsigned int uint;
typedef unsigned char uchar;

#define init_gl_window(WW, WH, TITLE) \
	glutInit(&argc, argv); \
	glutInitWindowSize(WW, WH); \
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); \
	glutCreateWindow(TITLE); \
	if (glewInit() != GLEW_OK) { \
		exit(1); \
	}

#define flush() glutSwapBuffers()

#define _c 255.0f
#define color_red glColor3f(255/_c, 0, 0)
#define color_green glColor3f(0, 255/_c, 0)
#define color_blue glColor3f(0, 0, 255/_c)
#define color_yellow glColor3f(255/_c, 255/_c, 0)
#define color_banana glColor3f(227/_c, 207/_c, 87/_c)
#define color_grey_blue glColor3f(176/_c, 224/_c, 230/_c)
#define color_1 glColor3f(56/_c, 94/_c, 15/_c)
#define color_2 glColor3f(61/_c, 145/_c, 64/_c)

static int WW = 960;
static int WH = 640;
#define aspect_ratio ((GLfloat)WW / (GLfloat)WH)

#define gl_check_error() \
	printf("gl-error=>%d", glGetError())

#endif