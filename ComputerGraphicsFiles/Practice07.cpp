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
	float x{ 0 };	//	중점 x좌표
	float y{ 0 };	//	중점 y좌표
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

	//	점의 크기 설정, 점의 크기는 glBegin()과 glEnd()밖에서 사용해야 한다고 함.
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for (int n = 0; n < MAXNUMBER_CIRCLE; ++n) {
		for (double i = 0.0; i < 2 * PI; i += 0.1) {
			if (g_circle[n].feature != NONE) {
				//	표기될 원만 (NONE이 아닌 특징을 가진) 그릴 것이다.

				if (g_circle[n].feature == MOVE_LOOP) {
					if (g_circle[n].radius > g_circle[n].maxRadius) {

						//	루프 특징을 가진 원은, 최대 반지름을 넘으면 초기화 시켜준다.
						g_circle[n].radius = 1;
					}
				}

				//	원주의 한 점이 윈도우 범위를 벗어나면 더 이상 그리지 않겠다.
				//	이때 계산을 복잡하게 여러 번 수행하는 것을 막아보고자, float 형태의 임시 저장변수, x와 y를 만든다.
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
	//	좌표계 변환 0,0이 좌하단
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
		//	window OS의 좌상단 0,0 좌표계->openGL 좌하단 0,0좌표계로
		g_circle[currentIndex].x = x;
		g_circle[currentIndex].y = WINDOW_HEIGHT - y;

		//	기본 특징은 NONE, 새로 발행시킨 특징은 원이 어떻게 움직일 지를 정해줌
		g_circle[currentIndex].feature = rand() % 2 + 1;
		g_circle[currentIndex].radius = 1;
		if (g_circle[currentIndex].feature == MOVE_LOOP) {
			g_circle[currentIndex].maxRadius = rand() % 500 + 1;
		}
		currentIndex++;
	}
}
