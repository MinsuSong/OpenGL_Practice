#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define RECT 1
#define TRIA 2

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);
int timeSpeed{ 1 };

struct Rect {
	float x;
	float y;
	float maxsize;
	float minsize;
	float currentsize;
	int sizeDirection{ 1 };
	int moveXDirection{ 1 };
	int moveYDirection{ 1 };
	int speed{ 0 };
	bool isGenerated{ false };
	int shape{ RECT };
};

Rect r[10];
int currentIndex{ 0 };

bool isPressedR{ true };
bool isPressedT{ false };


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice05");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(100 - timeSpeed, timerfunction, 1);
	glutMouseFunc(mousemovefunction);
	glutKeyboardFunc(keydownfunction);

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	for (int i = 0; i < 10; ++i) {
		if (r[i].isGenerated) {
			if (r[i].x - r[i].currentsize < 0) {
				r[i].x = r[i].currentsize;
				r[i].moveXDirection = 1;
			}
			else if (r[i].x + r[i].currentsize > 800) {
				r[i].x = 800 - r[i].currentsize;
				r[i].moveXDirection = 2;
			}
			else if (r[i].y - r[i].currentsize < 0) {
				r[i].y = r[i].currentsize;
				r[i].moveYDirection = 1;
			}
			else if (r[i].y + r[i].currentsize > 600) {
				r[i].y = 600 - r[i].currentsize;
				r[i].moveYDirection = 2;
			}

			if (r[i].moveXDirection == 1) {
				r[i].x += r[i].speed;
			}
			else {
				r[i].x -= r[i].speed;
			}

			if (r[i].moveYDirection == 1) {
				r[i].y += r[i].speed;
			}
			else {
				r[i].y -= r[i].speed;
			}


			if (r[i].sizeDirection == 1) {
				r[i].currentsize += 2;
			}
			else {
				r[i].currentsize -= 2;
			}
			glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);

			if (r[i].shape==RECT){
				glRectf(
					r[i].x - r[i].currentsize, r[i].y - r[i].currentsize,
					r[i].x + r[i].currentsize, r[i].y + r[i].currentsize
				);
			}
			else if (r[i].shape == TRIA) {
				glBegin(GL_TRIANGLES);
				glVertex2f(r[i].x, r[i].y + r[i].currentsize);
				glVertex2f(r[i].x - r[i].currentsize, r[i].y - r[i].currentsize);
				glVertex2f(r[i].x + r[i].currentsize, r[i].y - r[i].currentsize);
				glEnd();
			}
			
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
	glutTimerFunc(100 - timeSpeed, timerfunction, 1);
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
		r[currentIndex].speed = rand() % 10 + 1;
		r[currentIndex].moveXDirection = rand() % 2 + 1;
		r[currentIndex].moveYDirection = rand() % 2 + 1;

		if (isPressedR) {
			r[currentIndex].shape = RECT;
		}
		else if (isPressedT) {
			r[currentIndex].shape = TRIA;
		}
		currentIndex++;
	}
}

void keydownfunction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		timeSpeed += 1;
		printf("%d\n", timeSpeed);
		//glutTimerFunc(100 + timeSpeed, timerfunction, 1);
		break;
	case 'd':
	case 'D':
		timeSpeed -= 1;
		printf("%d\n", timeSpeed);
		//glutTimerFunc(100 + timeSpeed, timerfunction, 1);
		break;
	case 'r':
		isPressedR = true;
		isPressedT = false;
		break;
	case 't':
		isPressedR = false;
		isPressedT = true;
		break;

	default:
		break;
	}
}
