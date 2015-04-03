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

#endif