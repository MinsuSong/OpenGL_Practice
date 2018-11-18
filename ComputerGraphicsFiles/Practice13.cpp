#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "myHeader.h"

#define RECTANGLE	1
#define TRIANGLE	2

struct myPOINT {
	float x;
	float y;
	bool isGenerated{ false };
};

struct myFigure {
	float x;
	float y;
	float radian{ 0.f };
	Vector dirVector{ 0.f,0.f,0.f };
	int shape{ RECTANGLE };
};

#define NONE		-1
#define CIRCLE		0
#define SIN			1
#define SPRING		2
#define ZIGZAG		3
#define DRAW_MODE	4

#define RADIUS		150
#define MYFIGURE_SIZE	15

#define RIGHT 1
#define BIGGER 1
#define LEFT -1
#define SMALLER -1

#define SIN_MAX		50.f

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

//	declaration of callback functions
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);

float angle{ 0.f };
float scale{ 1.2f };

struct myTriangle {
	float x;
	float y;
	float z;
	Vector dir;
};

Vector XYtriangles[3];
Vector YZtriangles[3];
void drawAxis(void);
void drawSmallAxis(void);

bool isStarted = false;

int counterRotateY = 0;
int counterRotatey = 0;
int counterScaleS = 0;
int counterScales = 0;

int scaleDirection = BIGGER;

myTriangle xyTriangle{ 0.f,100.f,0.f };
myTriangle yzTriangle{ 0.f,100.f,0.f };

void initiate(void);

int currentLocationXY{ 1 };
int currentLocationYZ{ 1 };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice13");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(100, timerfunction, 1);
	glutMouseFunc(mousemovefunction);
	glutKeyboardFunc(keydownfunction);

	initiate();

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (!isStarted) {
		glTranslatef((float)MAX_WINDOW_X / (float)2, (float)MAX_WINDOW_Y / (float)2, 0.f);
		isStarted = true;
	}

	glPushMatrix();

	//	행렬 연산 누적
	for (int i = 0; i < counterRotateY; ++i) {
		glRotatef(10.f, 0.f, 1.f, 0.f);
	}
	for (int i = 0; i < counterRotatey; ++i) {
		glRotatef(-10.f, 0.f, 1.f, 0.f);
	}
	for (int i = 0; i < counterScaleS; ++i) {
		glScalef(1.5f, 1.5f, 1.f);
	}
	for (int i = 0; i < counterScales; ++i) {
		glScalef(0.5f, 0.5f, 1.f);
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.f, 100.f, 0.f);
	glVertex3f(100.f, -100.f, 0.f);
	glVertex3f(-100.f, -100.f, 0.f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.f, 100.f, 0.f);
	glVertex3f(0.f, -100.f, 100.f);
	glVertex3f(0.f, -100.f, -100.f);
	glEnd();

	glPushMatrix();
		glTranslatef(xyTriangle.x, xyTriangle.y,xyTriangle.z);
		glPushMatrix();
			glRotatef(angle, 0.f, 1.f, 0.f);
			glPushMatrix();
				glScalef(scale, scale, scale);
				glColor3f(1.0f, 1.0f, 0.0f);
				glBegin(GL_TRIANGLES);
				glVertex3f(0.f, 10.f, 0.f);
				glVertex3f(10.f, -10.f, 0.f);
				glVertex3f(-10.f, -10.f, 0.f);
				glEnd();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glRotatef(-90, 0.f, 1.f, 0.f);
		glPushMatrix();
		glTranslatef(xyTriangle.x, xyTriangle.y, xyTriangle.z);
			glPushMatrix();
			glRotatef(-angle, 0.f, 1.f, 0.f);
			glColor3f(0.f, 1.0f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(0.f, 10.f, 0.f);
			glVertex3f(10.f, -10.f, 0.f);
			glVertex3f(-10.f, -10.f, 0.f);
			glEnd();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();


	//glColor3f(0.f, 1.0f, 1.0f);
	//glPushMatrix();
	//glRotatef(90, 0.f, 1.f, 0.f);
	//glPushMatrix();
	//glRotatef(angle, 0.f, 1.f, 0.f);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(0.f + xyTriangle.x, 10.f + xyTriangle.y, 0.f + xyTriangle.z);
	//glVertex3f(10.f + xyTriangle.x, -10.f + xyTriangle.y, 0.f + xyTriangle.z);
	//glVertex3f(-10.f + xyTriangle.x, -10.f + xyTriangle.y, 0.f + xyTriangle.z);
	//glEnd();
	//glPopMatrix();
	//glPopMatrix();


	//glBegin(GL_TRIANGLES);
	//glVertex3f(0.f + yzTriangle.x, 10.f + yzTriangle.y, 0.f + yzTriangle.z);
	//glVertex3f(0.f + yzTriangle.x, -10.f + yzTriangle.y, 10.f + yzTriangle.z);
	//glVertex3f(0.f + yzTriangle.x, -10.f + yzTriangle.y, -10.f + yzTriangle.z);
	//glEnd();


	//	누적 연산 pop해서, 축 고정 시켜줄려고.
	glPopMatrix();

	drawAxis();

	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 0, 600.0, -400.0, 400.0);
}

void timerfunction(int value)
{
	{
		if (currentLocationXY == 4) {
			currentLocationXY = 1;
			xyTriangle.x = 0.f;
			xyTriangle.y = 100.f;
			xyTriangle.z = 0.f;
		}

		Vector center;
		center.x = xyTriangle.x;
		center.y = xyTriangle.y;
		center.z = xyTriangle.z;

		xyTriangle.dir.x = XYtriangles[currentLocationXY].x - xyTriangle.x;
		xyTriangle.dir.y = XYtriangles[currentLocationXY].y - xyTriangle.y;
		xyTriangle.dir.z = XYtriangles[currentLocationXY].z - xyTriangle.z;

		normalize(&xyTriangle.dir);
		//printf("%f,%f,%f\n", xyTriangle.dir.x, xyTriangle.dir.y, xyTriangle.dir.z);

		if (getDistance(center, XYtriangles[currentLocationXY]) < 2) {
			xyTriangle.x = XYtriangles[currentLocationXY].x;
			xyTriangle.y = XYtriangles[currentLocationXY].y;
			xyTriangle.z = XYtriangles[currentLocationXY].z;
			currentLocationXY++;
		}
		else {
			xyTriangle.x += xyTriangle.dir.x * 5;
			xyTriangle.y += xyTriangle.dir.y * 5;
			xyTriangle.z += xyTriangle.dir.z * 5;
		}
	}
	//////////////////////////////////////////////////////////
	//{
	//	if (currentLocationYZ == 4) {
	//		currentLocationYZ = 1;
	//		yzTriangle.x = 0.f;
	//		yzTriangle.y = 100.f;
	//		yzTriangle.z = 0.f;
	//	}
	//
	//	Vector center;
	//	center.x = yzTriangle.x;
	//	center.y = yzTriangle.y;
	//	center.z = yzTriangle.z;
	//
	//	yzTriangle.dir.x = YZtriangles[currentLocationYZ].x - yzTriangle.x;
	//	yzTriangle.dir.y = YZtriangles[currentLocationYZ].y - yzTriangle.y;
	//	yzTriangle.dir.z = YZtriangles[currentLocationYZ].z - yzTriangle.z;
	//
	//	normalize(&yzTriangle.dir);
	//	printf("%f,%f,%f\t%d\n", yzTriangle.dir.x, yzTriangle.dir.y, yzTriangle.dir.z,currentLocationYZ);
	//
	//	if (getDistance(center, YZtriangles[currentLocationYZ]) < 2) {
	//		yzTriangle.x = YZtriangles[currentLocationYZ].x;
	//		yzTriangle.y = YZtriangles[currentLocationYZ].y;
	//		yzTriangle.z = YZtriangles[currentLocationYZ].z;
	//		currentLocationYZ++;
	//	}
	//	else {
	//		yzTriangle.x += yzTriangle.dir.x * 5;
	//		yzTriangle.y += yzTriangle.dir.y * 5;
	//		yzTriangle.z += yzTriangle.dir.z * 5;
	//	}
	//}

	if (angle > 360) {
		angle = 0;
	}
	else {
		angle += 5;
	}

	if (scale > 1.8) {
		scale = 1.8f;
		scaleDirection = SMALLER;
	}
	else {
		scale += 0.05 * (float)scaleDirection;
	}

	if (scale < 0.5) {
		scale = 0.5f;
		scaleDirection = BIGGER;
	}
	else {
		scale += 0.05 * (float)scaleDirection;
	}

	glutPostRedisplay();
	glutTimerFunc(100, timerfunction, 1);

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
	case 's':
		counterScales++;
		break;
	case 'S':
		counterScaleS++;
		break;

	case 'y':	//	y 방향 음의 이동
		counterRotatey++;
		break;
	case 'Y':	//	y 방향 양의 이동
		counterRotateY++;
		break;
	case 'q':	//	프로그램 종료
	case 'Q':
		glutLeaveMainLoop();
	default:
		break;
	}

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
void drawSmallAxis(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-50.f, 0.f, 0.f);
	glVertex3f(50.f, 0.f, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -50.f, 0.f);
	glVertex3f(0.f, 50.f, 0.f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, -50.f);
	glVertex3f(0.f, 0.f, 50.f);
	glEnd();
}
void initiate(void)
{
	XYtriangles[0].x = 0.f;
	XYtriangles[0].y = 100.f;
	XYtriangles[0].z = 0.f;

	XYtriangles[1].x = 100.f;
	XYtriangles[1].y = -100.f;
	XYtriangles[1].z = 0.f;

	XYtriangles[2].x = -100.f;
	XYtriangles[2].y = -100.f;
	XYtriangles[2].z = 0.f;

	YZtriangles[0].x = 0.f;
	YZtriangles[0].y = 100.f;
	YZtriangles[0].z = 0.f;

	YZtriangles[1].x = 0.f;
	YZtriangles[1].y = -100.f;
	YZtriangles[1].z = 100.f;

	YZtriangles[2].x = 0.f;
	YZtriangles[2].y = -100.f;
	YZtriangles[2].z = -100.f;
}

/*
	삼각형 돌리고 회전시키고 여러가지 변환을 수행한 것을 Y축에 대해 로테이트 걸어서 마치 2개 만든 것처럼 효과를 냈음.

*/
