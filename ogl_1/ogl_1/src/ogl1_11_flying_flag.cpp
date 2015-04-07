//#define __exec

#include <stdio.h>
#include "common_header.h"
#include "GL/glew.h"
#include "GL/GLU.h"
#include "glut.h"

static void init_gl() {
	
}

static void on_draw() {
	
}

#ifdef __exec
int main(int argc, char *argv[]) {
	init_gl_window(WW, WH, "11");

	init_gl();
	glutDisplayFunc(on_draw);
	glutIdleFunc(on_draw);
	glutMainLoop();

	return 0;
}
#endif