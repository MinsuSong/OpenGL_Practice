#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		700

#define NONE				0
#define MOVE_STRAIGHT		1
#define MOVE_LOOP			2

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
	int feature{ NONE };
	float radius{ 1 };
	Color color;
	float maxRadius{ 0 };
};

Circle g_circle[10];
int currentIndex{ 0 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);




void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Practice07");
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
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for (int n = 0; n < MAXNUMBER_CIRCLE; ++n) {
		for (double i = 0.0; i < 2 * PI; i += 0.1) {
			if (g_circle[n].feature != NONE) {
				//	ǥ��� ���� (NONE�� �ƴ� Ư¡�� ����) �׸� ���̴�.

				if (g_circle[n].feature == MOVE_LOOP) {
					if (g_circle[n].radius > g_circle[n].maxRadius) {

						//	���� Ư¡�� ���� ����, �ִ� �������� ������ �ʱ�ȭ �����ش�.
						g_circle[n].radius = 1;
					}
				}

				//	������ �� ���� ������ ������ ����� �� �̻� �׸��� �ʰڴ�.
				//	�̶� ����� �����ϰ� ���� �� �����ϴ� ���� ���ƺ�����, float ������ �ӽ� ���庯��, x�� y�� �����.
				float x = g_circle[n].radius * cos(i) + g_circle[n].x;
				float y = g_circle[n].radius* sin(i) + g_circle[n].y;
				if (
					(0 <= x && x <= WINDOW_WIDTH)
					&&
					(0 <= y && y <= WINDOW_HEIGHT)
					) {
					glColor3f(g_circle[n].color.r, g_circle[n].color.g, g_circle[n].color.b);
					glVertex2f(x, y);
				}
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
		if (g_circle[i].feature != NONE) {
			g_circle[i].radius += 10;
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
		if (currentIndex == 9) {
			currentIndex = 0;
		}
		//	window OS�� �»�� 0,0 ��ǥ��->openGL ���ϴ� 0,0��ǥ���
		g_circle[currentIndex].x = x;
		g_circle[currentIndex].y = WINDOW_HEIGHT - y;

		//	�⺻ Ư¡�� NONE, ���� �����Ų Ư¡�� ���� ��� ������ ���� ������
		g_circle[currentIndex].feature = rand() % 2 + 1;
		g_circle[currentIndex].radius = 1;
		if (g_circle[currentIndex].feature == MOVE_LOOP) {
			g_circle[currentIndex].maxRadius = rand() % 500 + 1;
		}
		currentIndex++;
	}
}
