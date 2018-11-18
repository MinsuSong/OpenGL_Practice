#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);

struct Rect {
	float x;
	float y;
	float maxsize;
	float minsize;
	float currentsize;
	int sizeDirection{ 1 };
	bool isGenerated{ false };
};

Rect r[10];
int currentIndex{ 0 };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice04");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(100, timerfunction, 1);
	glutMouseFunc(mousemovefunction);

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	
	for (int i = 0; i < 10; ++i) {
		if (r[i].isGenerated) {
			if (r[i].sizeDirection == 1) {
				r[i].currentsize += 2;
			}
			else {
				r[i].currentsize -= 2;
			}
			glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
			glRectf(
				r[i].x - r[i].currentsize, r[i].y - r[i].currentsize,
				r[i].x + r[i].currentsize, r[i].y + r[i].currentsize
			);
		}
	}

	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 0, 600.0, -1.0, 1.0);
}

void timerfunction(int value)
{
	for (int i = 0; i < 10; ++i) {
		if (r[i].currentsize <= r[i].minsize) {
			r[i].currentsize = r[i].minsize;
			r[i].sizeDirection = +1;
		}
		else if (r[i].currentsize >= r[i].maxsize) {
			r[i].currentsize = r[i].maxsize;
			r[i].sizeDirection = -1;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(100, timerfunction, 1);
}

void mousemovefunction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentIndex == 10) {
			currentIndex = 0;
		}
		r[currentIndex].x = x;
		r[currentIndex].y = 600 - y;
		r[currentIndex].maxsize = rand() % 25 + 1;
		r[currentIndex].minsize = rand() % 5 + 1;
		r[currentIndex].currentsize = rand() % 25 + 1;
		r[currentIndex].isGenerated = true;
		currentIndex++;
	}
}
