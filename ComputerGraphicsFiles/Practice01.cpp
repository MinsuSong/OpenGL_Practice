#include <gl/freeglut.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

GLint p1[2] = { 1,0 };
GLint p2[2] = { -1,0 };
GLint p3[2] = { 0,1 };
GLint p4[2] = { 0,-1 };

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice01");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.5f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glVertex2iv(p1);
	glVertex2iv(p2);
	glVertex2iv(p3);
	glVertex2iv(p4);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 0.0f, 0.0f);
	glVertex3f(0.1, 0.0f, 0.0f);
	glVertex3f(-0.1, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.14, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(-1.0f, 1.0f, -1.0f + 0.1f, 1.0f - 0.1f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(-1.0f, -1.0f, -1.0f + 0.1f, -1.0f + 0.1f);

	glColor3f(0.0f, 1.0f, 1.0f);
	glRectf(+1.0f - 0.1f, 1.0f, 1.0f, 1.0f - 0.1f);

	glColor3f(1.0f, 0.0f, 1.0f);
	glRectf(+1.0f - 0.1f, -1.0f, 1.0f, -1.0f + 0.1f);


	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


