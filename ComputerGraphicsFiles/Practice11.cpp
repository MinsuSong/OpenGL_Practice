#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "myHeader.h"

#define SIN_MAX	50
#define RIGHT 1
#define LEFT -1

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

//	declaration of callback functions
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);

void drawGraphSin(void);
void drawGraphSpring(void);
void drawGraphRibbon(void);
void drawGraphRectangle(void);

void drawAxis(void);

bool isStarted = false;

int counterMoveX = 0;
int counterMovex = 0;
int counterMoveY = 0;
int counterMovey = 0;
int counterRotateR = 0;
int counterRotater = 0;
int counterScaleS = 0;
int counterScales = 0;

bool isAkey = false;
bool isakey = false;
bool isPaused = true;

int triangleDirection = RIGHT;

float g_sinTheta = 0.f;
float g_springTheta = 0.f;
float g_trianglePoint = 0.f;
float g_rectanglePoint = 0.f;

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice11");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(100, timerfunction, 1);
	glutMouseFunc(mousemovefunction);
	glutKeyboardFunc(keydownfunction);

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (!isStarted) {
		glTranslatef((float)MAX_WINDOW_X / (float)2, (float)MAX_WINDOW_Y / (float)2, 0.f);
	}

	glPushMatrix();
	
	for (int i = 0; i < counterMoveX; ++i) {
		glTranslatef(10.f, 0.f, 0.f);
	}for (int i = 0; i < counterMovex; ++i) {
		glTranslatef(-10.f, 0.f, 0.f);
	}for (int i = 0; i < counterMoveY; ++i) {
		glTranslatef(0.f, 10.f, 0.f);
	}for (int i = 0; i < counterMovey; ++i) {
		glTranslatef(0.f, -10.f, 0.f);
	}for (int i = 0; i < counterRotateR; ++i) {
		glRotatef(-10, 0.f, 1.f, 0.f);
	}for (int i = 0; i < counterRotater; ++i) {
		glRotatef(10, 0.f, 1.f, 0.f);
	}for (int i = 0; i < counterScaleS; ++i) {
		glScalef(1.25f, 1.f, 1.0f);
	}for (int i = 0; i < counterScales; ++i) {
		glScalef(0.5f, 1.f, 1.f);
	}
	
	drawGraphSin();
	drawGraphSpring();
	drawGraphRibbon();
	drawGraphRectangle();
	
	glPopMatrix();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-(float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glVertex3f((float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -(float)MAX_WINDOW_Y / (float)2, 0.f);
	glVertex3f(0.f, (float)MAX_WINDOW_Y / (float)2, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, (float)MIN_WINDOW_Z);
	glVertex3f(0.f, 0.f, (float)MAX_WINDOW_Z);
	glEnd();

	isStarted = true;

	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 0, 600.0, -400.0, 400.0);
}

void timerfunction(int value)
{
	if (isPaused) {
		return;
	}
	if (isakey) {
		if (g_rectanglePoint < -40) {
			g_rectanglePoint = 0.f;
		}
		else {
			g_rectanglePoint--;
		}

		g_sinTheta -= 0.5;
		g_springTheta -= 0.5;
	}

	if (isAkey) {
		if (g_rectanglePoint > 40) {
			g_rectanglePoint = 0.f;
		}
		else {
			g_rectanglePoint++;
		}
		g_sinTheta += 0.5;
		g_springTheta += 0.5;
	}

	if (triangleDirection == RIGHT) {
		if (g_trianglePoint > 50) {
			g_trianglePoint = 50;
			triangleDirection = LEFT;
		}
		else {
			g_trianglePoint+=2;
		}
	}
	else {
		if (g_trianglePoint < 0) {
			g_trianglePoint = 0;
			triangleDirection = RIGHT;
		}
		else {
			g_trianglePoint-=2;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(50, timerfunction, 1);
	
}

void mousemovefunction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}
}

void keydownfunction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':	//	x 방향 음의 이동
		counterMovex++;
		break;
	case 'X':	//	x 방향 양의 이동
		counterMoveX++;
		break;
	case 'y':	//	y 방향 음의 이동
		counterMovey++;
		break;
	case 'Y':	//	y 방향 양의 이동
		counterMoveY++;
		break;
	case 's':	//	x축 기준 확대
		counterScales++;
		break;
	case 'S':	//	x축 기준 축소
		counterScaleS++;
		break;
	case 'r':	//	x 방향 음 특정값 이동
		counterRotater++;
		break;
	case 'R':	//	x 방향 양 특정값 이동
		counterRotateR++;
		break;
	case 'a':	//	음의 방향 이동 애니메이션
		if (isPaused) glutTimerFunc(50, timerfunction, 1);
		isAkey = false;
		isakey = true;
		isPaused = false;
		triangleDirection = LEFT;
		break;
	case 'A':	//	양의 방향 이동 애니메이션
		if (isPaused) glutTimerFunc(50, timerfunction, 1);
		isAkey = true;
		isakey = false;
		isPaused = false;
		triangleDirection = RIGHT;
		break;
	case 'T':	//	애니메이션 멈추기
		isAkey = false;
		isakey = false;
		isPaused = true;
		break;
	case 'q':	//	프로그램 종료
	case 'Q':
		glutLeaveMainLoop();
	default:
		break;
	}

	printf("%c", key);
	glutPostRedisplay();
}

void drawAxis(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-(float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glVertex3f((float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -(float)MAX_WINDOW_Y / (float)2, 0.f);
	glVertex3f(0.f, (float)MAX_WINDOW_Y / (float)2, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, (float)MIN_WINDOW_Z);
	glVertex3f(0.f, 0.f, (float)MAX_WINDOW_Z);
	glEnd();
}

void drawGraphSin(void)
{
	glColor3f(1.0f, 1.f, 1.f);
	for (float i = -(float)8 * PI; i < (float)8 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex2f(i * (float)5, sin(i + g_sinTheta) * (float)SIN_MAX + (float)MAX_WINDOW_Y * (float)3 / (float)8);
		glEnd();
	}
	
}

void drawGraphSpring(void)
{
	glColor3f(1.0f, 1.f, 1.f);
	for (float i = -(float)8 * PI; i < (float)8 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex2f(25 * cos(i + g_springTheta) + i * 10, 25 * sin(i + g_springTheta) + (float)MAX_WINDOW_Y * (float)1 / (float)8);
		glEnd();
	}
}

void drawGraphRibbon(void)
{
	glColor3f(1.0f, 1.f, 1.f);
	glPushMatrix();
	glTranslatef(0.f, -(float)MAX_WINDOW_Y / (float)8, 0.f);
	glBegin(GL_TRIANGLES);
	glVertex3f(10.f * 10.f + g_trianglePoint, 10.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(10.f * 10.f + g_trianglePoint, -10.f, 0.f);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.f, -(float)MAX_WINDOW_Y / (float)8, 0.f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-10.f * 10.f - g_trianglePoint, 10.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(-10.f * 10.f - g_trianglePoint, -10.f, 0.f);
	glEnd();
	glPopMatrix();
}

void drawGraphRectangle(void)
{
	glColor3f(1.f, 1.f, 1.f);
	glPushMatrix();
	glTranslatef(0.f, -(float)MAX_WINDOW_Y * (float)3 / (float)8, 0.f);
	glBegin(GL_POLYGON);
	glVertex3f(160.f, 10.f, 0.f);
	glVertex3f(-160.f, 10.f, 0.f);
	glVertex3f(-160.f, -10.f, 0.f);
	glVertex3f(160.f, -10.f, 0.f);
	glEnd();
	glPopMatrix();
	for (float i = 0; i < 14; ++i) {
		glPushMatrix();
		glTranslatef(-(float)40 * 7 + i * (float)40 + g_rectanglePoint, -(float)MAX_WINDOW_Y * (float)3 / (float)8, 0.f);
		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_POLYGON);
		glVertex3f(10.f, 10.f, 0.f);
		glVertex3f(-10.f, 10.f, 0.f);
		glVertex3f(-10.f, -10.f, 0.f);
		glVertex3f(10.f, -10.f, 0.f);
		glEnd();
		glPopMatrix();
	}
}
