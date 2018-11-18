#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define RECT 1

#define LEFT	-1
#define RIGHT	1
#define UP		1
#define DOWN	-1
#define BIGGER	1
#define SMALLER	-1
#define MAX_NUM	30


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void mousemotionfunction(int x, int y);

struct Color{
	float r;
	float g;
	float b;
};

struct Rect {
	int x;
	int y;
	Color color;
	int maxsize;
	int minsize;
	int currentsize;
	int variablesize{ 0 };
	int sizeDirection{ BIGGER };
	int moveXDirection{ LEFT };
	int moveYDirection{ UP };
	int shapeDirection{ BIGGER };
	int speed{ 0 };
	bool isCollide{ false };
	int shape{ RECT };
};

Rect r[100];
Rect g_mouseRect;

bool g_isClicked{ false };


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice10");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	//	callback func s
	glutTimerFunc(100, timerfunction, 1);
	glutMouseFunc(mousemovefunction);
	glutMotionFunc(mousemotionfunction);



	for (int i = 0; i < MAX_NUM; ++i) {
		r[i].maxsize = rand() % 2 + 10;
		r[i].minsize = rand() % 2 + 8;
		r[i].currentsize = rand() % 9 + 1;
		r[i].isCollide = false;
		r[i].speed = rand() % 10 + 1;
		r[i].moveXDirection = rand() % 2 + 1;
		r[i].moveYDirection = rand() % 2 + 1;
		r[i].x = rand() % 800 - r[i].currentsize + r[i].currentsize;
		r[i].y = rand() % 600 - r[i].currentsize + r[i].currentsize;
		r[i].color.r = (double)rand() / RAND_MAX;
		r[i].color.g = (double)rand() / RAND_MAX;
		r[i].color.b = (double)rand() / RAND_MAX;
	}

	glutMainLoop();


}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < MAX_NUM; ++i) {
		//	collide check
		if (g_isClicked) {
			if (!r[i].isCollide) {
				if ((g_mouseRect.x + 20 >= r[i].x - r[i].currentsize) && (g_mouseRect.x - 20 <= r[i].x + r[i].currentsize)
					&& (g_mouseRect.y + 20 >= r[i].y - r[i].currentsize) && (g_mouseRect.y - 20 <= r[i].y + r[i].currentsize)) {
					r[i].isCollide = true;
				}
			}
		}

		if (r[i].x - r[i].currentsize < 0) {
			r[i].x = r[i].currentsize;
			r[i].moveXDirection = RIGHT;
		}
		else if (r[i].x + r[i].currentsize > 800) {
			r[i].x = 800 - r[i].currentsize;
			r[i].moveXDirection = LEFT;
		}
		else if (r[i].y - r[i].currentsize < 0) {
			r[i].y = r[i].currentsize;
			r[i].moveYDirection = UP;
		}
		else if (r[i].y + r[i].currentsize > 600) {
			r[i].y = 600 - r[i].currentsize;
			r[i].moveYDirection = DOWN;
		}


		r[i].x += r[i].speed * r[i].moveXDirection;
		r[i].y += r[i].speed * r[i].moveYDirection;



		if (r[i].sizeDirection == BIGGER) {
			r[i].currentsize += 0.1;
		}
		else {
			r[i].currentsize -= 0.1;
		}
		

		if (!r[i].isCollide) {
			glColor3f(r[i].color.r, r[i].color.g, r[i].color.b);
			glRectf(
				r[i].x - r[i].currentsize, r[i].y - r[i].currentsize,
				r[i].x + r[i].currentsize, r[i].y + r[i].currentsize
			);
		}
		else {
			glBegin(GL_POLYGON);
			glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
			glVertex2f(r[i].x - r[i].currentsize - r[i].variablesize, r[i].y - r[i].currentsize);
			glVertex2f(r[i].x + r[i].currentsize + r[i].variablesize, r[i].y - r[i].currentsize);
			glVertex2f(r[i].x + r[i].currentsize - r[i].variablesize, r[i].y + r[i].currentsize);
			glVertex2f(r[i].x - r[i].currentsize + r[i].variablesize, r[i].y + r[i].currentsize);
			glEnd();
		}
	}

	if (g_isClicked) {
		glColor3f(1.f, 1.f, 1.f);
		glRectf(g_mouseRect.x - 20, g_mouseRect.y - 20, g_mouseRect.x + 20, g_mouseRect.y + 20);
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
	for (int i = 0; i < MAX_NUM; ++i) {
		if (r[i].currentsize <= r[i].minsize) {
			r[i].currentsize = r[i].minsize;
			r[i].sizeDirection = BIGGER;
		}
		else if (r[i].currentsize >= r[i].maxsize) {
			r[i].currentsize = r[i].maxsize;
			r[i].sizeDirection = SMALLER;
		}

		if (r[i].isCollide) {

			if (r[i].shapeDirection == BIGGER) {
				if (r[i].x - r[i].currentsize +r[i].variablesize < r[i].x) {
					r[i].variablesize++;
				}
				else {
					r[i].shapeDirection = SMALLER;
				}
			}
			else if (r[i].shapeDirection == SMALLER) {
				if (r[i].variablesize > 0) {
					r[i].variablesize--;
				}
				else {
					r[i].shapeDirection = BIGGER;
					r[i].isCollide = false;
				}
			}
		}
	}


	glutPostRedisplay();
	glutTimerFunc(100, timerfunction, 1);
}

void mousemovefunction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		g_isClicked = true;
		g_mouseRect.x = x;
		g_mouseRect.y = 600 - y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		g_isClicked = false;
	}
}

void mousemotionfunction(int x, int y)
{
	g_mouseRect.x = x;
	g_mouseRect.y = 600 - y;
}
