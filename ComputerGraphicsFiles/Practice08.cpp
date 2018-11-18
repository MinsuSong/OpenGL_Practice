#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		700

#define NONE				0
#define MOVE_STRAIGHT		1
#define MOVE_LOOP			2

#define TO_LEFT				1
#define TO_RIGHT			2
#define NONE				0

#define PI					3.141592


#define MAXNUMBER_CIRCLE	9

struct Color {
	float r;
	float g;
	float b;
};

struct Circle {
	float x{ 0 };	//	���� x��ǥ
	float y{ 0 };	//	���� y��ǥ
	float radius{ 0 };
	float maxRadius{ 0 };
	float radian1st{ 0.0 };
	float radian2nd{ 0.0 };
	bool isGenerated{ false };
	int shape{ NONE };
	bool isReversed{ false };
	Color color;
};

Circle g_circle[10];
int currentIndex{ 0 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);


static float r = 0.0;


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Practice08");
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

	//	���� ũ�� ����, ���� ũ��� glBegin()�� glEnd()�ۿ��� ����ؾ� �Ѵٰ� ��.
	glPointSize(1.0f);

	glBegin(GL_POINTS);
	for (int n = 0; n < MAXNUMBER_CIRCLE; ++n) {

		glColor3f(g_circle[n].color.r, g_circle[n].color.g, g_circle[n].color.b);
	  
		if (g_circle[n].shape == TO_RIGHT) {
			for (float theta = 0.0; theta < g_circle[n].radian1st; theta += 0.1) {
				float x = 2 * theta * cos(theta) + g_circle[n].x;
				float y = 2 * theta * sin(theta) + g_circle[n].y;
				glVertex2f(x, y);
			}
			for (float theta = 6.f * PI; theta > g_circle[n].radian2nd; theta -= 0.1) {
				float x = 2 * theta * cos(theta + PI) + g_circle[n].x + 2 * 2 * 6 * PI;
				float y = 2 * theta * sin(theta + PI) + g_circle[n].y;
				glVertex2f(x, y);
			}
		}
		else if (g_circle[n].shape == TO_LEFT) {
			for (float theta = 0.0; theta < g_circle[n].radian1st; theta += 0.1) {
				float x = 2 * theta * cos(theta + PI) + g_circle[n].x;
				float y = 2 * theta * sin(theta + PI) + g_circle[n].y;
				glVertex2f(x, y);
			}

			for (float theta = 6.f * PI; theta > g_circle[n].radian2nd; theta -= 0.1) {
				float x = 2 * theta * cos(theta) + g_circle[n].x - 2 * 2 * 6 * PI;
				float y = 2 * theta * sin(theta) + g_circle[n].y;
				glVertex2f(x, y);
			}
		}


	}
	
	glEnd();




	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1.0, 1.0);
	//	��ǥ�� ��ȯ 0,0�� ���ϴ�
}

void timerfunction(int value)
{
	for (int i = 0; i < MAXNUMBER_CIRCLE; ++i) {
		if (g_circle[i].isGenerated) {
			if (g_circle[i].radian1st < 6 * PI) {
				g_circle[i].radian1st += 0.5;
			}
			else {
				g_circle[i].isReversed = true;
			}

			if (g_circle[i].isReversed) {
				if (g_circle[i].radian2nd > 0) {
					g_circle[i].radian2nd -= 0.5;
				}
			}


			


			//	���� �ýð��� ����
			g_circle[i].color.r = (double)rand() / RAND_MAX;
			g_circle[i].color.g = (double)rand() / RAND_MAX;
			g_circle[i].color.b = (double)rand() / RAND_MAX;
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
		//	window OS�� �»�� 0,0 ��ǥ��->openGL ���ϴ� 0,0��ǥ���
		g_circle[currentIndex].x = x;
		g_circle[currentIndex].y = WINDOW_HEIGHT - y;

		//	������,�ִ� ������ �� �� �ʱⰪ ����
		g_circle[currentIndex].radius = 0;
		g_circle[currentIndex].radian1st = 0.0;
		g_circle[currentIndex].radian2nd = 6 * PI;
		g_circle[currentIndex].maxRadius = 20;
		g_circle[currentIndex].isGenerated = true;
		g_circle[currentIndex].shape = rand() % 2 + 1;
		g_circle[currentIndex].isReversed = false;
		currentIndex++;
	}
}
