#ifndef common_header_h
#define common_header_h

#define init_gl_window(WW, WH, TITLE) \
	glutInit(&argc, argv); \
	glutInitWindowSize(WW, WH); \
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); \
	glutCreateWindow(TITLE); \
	if (glewInit() != GLEW_OK) { \
		exit(1); \
	}

#define flush() glutSwapBuffers()

#define color_red glColor3f(1, 0, 0)
#define color_green glColor3f(0, 1, 0)
#define color_blue glColor3f(0, 0, 1)
#define color_yellow glColor3f(1, 1, 0)

static const int WW = 960;
static const int WH = 640;
#define aspect_ratio ((GLfloat)WW / (GLfloat)WH)

#endif