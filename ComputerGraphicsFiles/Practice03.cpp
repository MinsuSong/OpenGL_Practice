#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

int x = 100;
int y = 100;

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Practice03");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 3; ++i) {
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2f(600 / 3 * (i + 1), 0);
		glVertex2f(600 / 3 * (i + 1), 600);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(0, 600 / 3 * (i + 1));
		glVertex2f(600, 600 / 3 * (i + 1));
		glEnd();
	}

	int shape = rand() % 4;
	int shapedir = 1;


	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (shape == 0) {
				glBegin(GL_LINE_STRIP);
				glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
				glVertex2f(x - 50, y - 50);
				glVertex2f(x + 50, y + 50);
				glEnd();
				x += 200;
				shapedir = 1;
				shape++;
				
				
			}
			else if (shape == 1) {
				glBegin(GL_TRIANGLES);
				glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
				glVertex2f(x, y + 25);
				glVertex2f(x - 25, y - 25);
				glVertex2f(x + 25, y - 25);
				glEnd();
				x += 200;
				if (shapedir == 1) {
					shape++;
				}
				else {
					shape--;
				}
			}
			else if (shape == 2) {
				glBegin(GL_POLYGON);
				glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
				glVertex2f(x - 50, y + 50);
				glVertex2f(x - 50, y - 50);
				glVertex2f(x + 50, y - 50);
				glVertex2f(x + 50, y + 50);
				glEnd();
				x += 200;
				if (shapedir == 1) {
					shape++;
				}
				else {
					shape--;
				}
			}
			else {
				glBegin(GL_POLYGON);
				glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
				glVertex2f(x, y + 50);
				glVertex2f(x - 50, y + 15);
				glVertex2f(x - 30, y - 50);
				glVertex2f(x + 30, y - 50);
				glVertex2f(x + 50, y + 15);
				glEnd();
				x += 200;
				shapedir = 0;
				shape--;
				
			}
		}
		y += 200;
		x = 100;
	}
	



	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 600, 0, 600.0, -1.0, 1.0);
}