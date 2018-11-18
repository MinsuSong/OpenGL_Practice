#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "myHeader.h"

#define RECTANGLE	1
#define TRIANGLE	2

Vector xAxis{ 1.f,0.f,0.f };

struct POINTs {
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

#define RADIUS			150.f
#define MYFIGURE_SIZE	15

#define RIGHT 1
#define LEFT -1

#define SIN_MAX		50.f

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

Vector v;

//	declaration of callback functions
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);

void drawGraphCircle(void);
void drawGraphSin(void);
void drawGraphSpring(void);
void drawGraphZigzag(void);
void drawGraphFreeDraw(void);

void drawTriangle(void);
void drawRectangle(void);

void drawAxis(void);

void drawMyFigure(void);

bool isStarted = false;

int counterRotateY = 0;
int counterRotatey = 0;

int triangleDirection = RIGHT;

int currentIndex = 0;

int mode = NONE;

int currentVectorIndex = 1;

bool freedrawStart = false;

POINTs freedrawArray[5]{ 0 };
myFigure myfigure;

int currentLocation = 0;

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice12another");
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
		isStarted = true;
	}

	glPushMatrix();

	//	��� ���� ����
	for (int i = 0; i < counterRotateY; ++i) {
		glRotatef(10.f, 0.f, 1.f, 0.f);
	}for (int i = 0; i < counterRotatey; ++i) {
		glRotatef(-10.f, 0.f, 1.f, 0.f);
	}

	if (mode == CIRCLE) {
		drawGraphCircle();
	}
	else if (mode == SIN) {
		drawGraphSin();
	}
	else if (mode == SPRING) {
		drawGraphSpring();
	}
	else if (mode == ZIGZAG) {
		drawGraphZigzag();
	}
	else if (mode == DRAW_MODE) {
		drawGraphFreeDraw();
	}

	//	���� ���� pop�ؼ�, �� ���� �����ٷ���.
	
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
	if (mode == CIRCLE) {
		if (myfigure.radian > 2 * PI) {
			myfigure.radian = 0.f;
			myfigure.x = (float)RADIUS;
			myfigure.y = 0.f;
		}
		else {
			myfigure.radian += 0.1f;
		}
		myfigure.x = (float)RADIUS * cos(myfigure.radian);
		myfigure.y = (float)RADIUS * sin(myfigure.radian);
		myfigure.dirVector.x = RADIUS * (cos(myfigure.radian + 0.1f) - cos(myfigure.radian));
		myfigure.dirVector.y = RADIUS * (sin(myfigure.radian + 0.1f) - sin(myfigure.radian));
		myfigure.dirVector.z = 0.f;

		normalize(&myfigure.dirVector);

		printf("%f\n", myfigure.radian * 180.f / PI);
	}

	else if (mode == SIN) {
		if (myfigure.radian > 16 * PI) {
			myfigure.radian = 0.f;
		}
		else {
			myfigure.radian += 0.25;
		}
		myfigure.x = myfigure.radian * SIN_MAX / (float)4;
		myfigure.y = sin(myfigure.radian)* SIN_MAX * (float)2;
		myfigure.dirVector.x = (myfigure.radian + 0.25 - myfigure.radian) * SIN_MAX / (float)4;
		myfigure.dirVector.y = (sin(myfigure.radian + 0.25) - sin(myfigure.radian))* SIN_MAX * (float)2;
		normalize(&myfigure.dirVector);
	}

	else if (mode == SPRING) {
		if (myfigure.radian > 16 * PI) {
			myfigure.radian = 0.f;
		}
		else {
			myfigure.radian += 0.5;
		}
		myfigure.x = SIN_MAX * cos(myfigure.radian) + myfigure.radian * 10;
		myfigure.y = SIN_MAX * sin(myfigure.radian);
		myfigure.dirVector.x =	SIN_MAX * cos(myfigure.radian + 0.5) + (myfigure.radian + 0.5) * 10 -
								(SIN_MAX * cos(myfigure.radian) + (myfigure.radian) * 10);
		myfigure.dirVector.y = SIN_MAX * sin(myfigure.radian + 0.5) - (SIN_MAX * sin(myfigure.radian));
		normalize(&myfigure.dirVector);
	}

	else if (mode == ZIGZAG) {
		if (myfigure.x > 600) {
			myfigure.x = 0.f;
			myfigure.y = 200;
			currentLocation = 0;
		}

		if (myfigure.y > 200) {
			currentLocation++;
			myfigure.y = 200;
			myfigure.x = currentLocation * 40;
			myfigure.dirVector.x = 40 - 0;
			myfigure.dirVector.y = -200 - 200;
		}
		else if (myfigure.y < -200) {
			currentLocation++;
			myfigure.y = -200;
			myfigure.x = currentLocation * 40;
			myfigure.dirVector.x = 80 - 40;
			myfigure.dirVector.y = 200 + 200;
		}
		normalize(&myfigure.dirVector);
		myfigure.x = myfigure.x + myfigure.dirVector.x * 10.f;
		myfigure.y = myfigure.y + myfigure.dirVector.y * 10.f;
	}

	else if (mode == DRAW_MODE) {
		if (freedrawStart) {
			if (currentVectorIndex == 5) {
				currentVectorIndex = 1;
				myfigure.x = freedrawArray[currentVectorIndex - 1].x;
				myfigure.y = freedrawArray[currentVectorIndex - 1].y;
			}
			//printf("(%f %f)\t(%f %f)\n", freedrawArray[currentVectorIndex - 1].x, freedrawArray[currentVectorIndex - 1].y, myfigure.x, myfigure.y);
			Vector target;
			target.x = freedrawArray[currentVectorIndex].x;
			target.y = freedrawArray[currentVectorIndex].y;
			target.z = 0.f;

			Vector mf;
			mf.x = myfigure.x;
			mf.y = myfigure.y;
			mf.z = 0.f;

			myfigure.dirVector.x = freedrawArray[currentVectorIndex].x - freedrawArray[currentVectorIndex - 1].x;
			myfigure.dirVector.y = freedrawArray[currentVectorIndex].y - freedrawArray[currentVectorIndex-1].y;
			myfigure.dirVector.z = 0.f;

			normalize(&myfigure.dirVector);

			if (getDistance(target, mf) < 10.f) {
				myfigure.x = freedrawArray[currentVectorIndex].x;
				myfigure.y = freedrawArray[currentVectorIndex].y;
				++currentVectorIndex;
			}
			else {
				myfigure.x += myfigure.dirVector.x * 10.f;
				myfigure.y += myfigure.dirVector.y * 10.f;
			}
		}
	}

	printf("%f\t%f\t%d\n", myfigure.dirVector.x, myfigure.dirVector.y, currentVectorIndex);

	glutPostRedisplay();
	glutTimerFunc(50, timerfunction, 1);

}

void mousemovefunction(int button, int state, int x, int y)
{
	if (mode == DRAW_MODE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (currentIndex < 5) {
				/* ������ǥ��� ���ϴ����� �����ߴµ�, �̵���ȯ���� ���� ȭ���� ���߾Ӻο� ���������Ƿ� �� ���̸�ŭ �����̵�
				   �̶� ������ OS���� �ִ� ���콺 ��ǥ�� ��ǥ��� �»������ �ν��ϹǷ� �� ���̸� ����Ѵ�. */
				freedrawArray[currentIndex].x = x - (float)MAX_WINDOW_X / (float)2;
				freedrawArray[currentIndex].y = (float)MAX_WINDOW_Y - (float)y - (float)MAX_WINDOW_Y / (float)2;
				freedrawArray[currentIndex].isGenerated = true;
				currentIndex++;
				if (currentIndex == 5) {
					/*	5�� ���� ��� ����ٸ�?
						ù ������ ������ ���������� �ʱ�ȭ */
					freedrawStart = true;
					myfigure.x = freedrawArray[0].x;
					myfigure.y = freedrawArray[0].y;
				}
			}
		}
	}
}

void keydownfunction(unsigned char key, int x, int y)
{
	/* ���� �� ���̵��� ������ ��ġ���� ���úο��� �ʱ�ȭ �� �� */
	switch (key)
	{
	case '0':
		mode = CIRCLE;
		myfigure.radian = 0.f;
		break;
	case '1':
		mode = SIN;
		myfigure.radian = 0.f;
		break;
	case '2':
		mode = SPRING;
		myfigure.radian = 0.f;
		break;
	case '3':
		mode = ZIGZAG;
		myfigure.x = 0.f;
		myfigure.y = 200.f;
		myfigure.dirVector.x = 40 - 0;
		myfigure.dirVector.y = -200 - 200;
		currentLocation = 0;
		normalize(&myfigure.dirVector);
		break;
	case '4':
		mode = DRAW_MODE;
		myfigure.x = 0.f;
		myfigure.dirVector.x = 0;
		myfigure.dirVector.y = 1;
		myfigure.y = 0.f;
		freedrawStart = false;
		for (int i = 0; i < 5; ++i) {
			freedrawArray[i].isGenerated = false;
		}
		currentIndex = 0;
		currentVectorIndex = 1;
		break;

		/* ���� Ű Ŀ�ǵ� */
	case 'r':
	case 'R':
		myfigure.shape = RECTANGLE;
		break;
	case 't':
	case 'T':
		myfigure.shape = TRIANGLE;
		break;

		/* y�࿡ ���� ȸ�� */
	case 'y':	//	���� �̵�
		counterRotatey++;
		break;
	case 'Y':	//	���� �̵�
		counterRotateY++;
		break;

		/* ���α׷� ���� Ŀ�ǵ� */
	case 'q':
	case 'Q':
		glutLeaveMainLoop();

	default:
		break;
	}
	glutPostRedisplay();
}


void drawAxis(void)
{
	glBegin(GL_LINES);	//	X ��
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-(float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glVertex3f((float)MAX_WINDOW_X / (float)2, 0.f, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Y ��
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -(float)MAX_WINDOW_Y / (float)2, 0.f);
	glVertex3f(0.f, (float)MAX_WINDOW_Y / (float)2, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z ��
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, (float)MIN_WINDOW_Z);
	glVertex3f(0.f, 0.f, (float)MAX_WINDOW_Z);
	glEnd();
}

void drawGraphCircle(void)
{
	glPointSize(1.f);
	glColor3f(1.f, 1.f, 1.f);
	for (float i = 0.f; i < 2.f * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex3f((float)RADIUS * cos(i), (float)RADIUS * sin(i), 0.f);
		glEnd();
	}

	glPushMatrix();
		glPushMatrix();
			glTranslated(myfigure.x, myfigure.y, 0.f);
			//glRotatef(myfigure.radian / PI * 180.f, 0.f, 0.f, 1.f);

			if (myfigure.dirVector.y >= 0) {
				glRotatef(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI, 0.f, 0.f, 1.f);
			}
			else {
				glRotatef(-(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI), 0.f, 0.f, 1.f);
			}

			drawMyFigure();
		glPopMatrix();
	glPopMatrix();
}

void drawGraphSin(void)
{
	glPushMatrix();
		glTranslatef(-300.f, 0.f, 0.f);				//	���� �׷��� ������ ���� ��ǥ (-300, 0)
		glPointSize(1.f);
		glColor3f(1.f, 1.f, 1.f);

		for (float i = 0.f; i < (float)16 * PI; i += 0.01) {	//	������ ������ ���� �׷��� ����
			glBegin(GL_POINTS);
			glVertex3f(i * SIN_MAX / (float)4, sin(i)* SIN_MAX * (float)2, 0.f);
			glEnd();
		}
		glPushMatrix();
			glTranslated(myfigure.x, myfigure.y, 0.f);
			if (myfigure.dirVector.y >= 0) {
				glRotatef(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI, 0.f, 0.f, 1.f);
			}
			else {
				glRotatef(-(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI), 0.f, 0.f, 1.f);
			}
			
			drawMyFigure();
		glPopMatrix();
	glPopMatrix();
}

void drawGraphSpring(void)
{
	glPushMatrix();
		glTranslatef(-300.f, 0.f, 0.f);				//	������ �׷��� ������ ���� ��ǥ (-300, 0)
		glPointSize(1.f);
		glColor3f(1.f, 1.f, 1.f);

		for (float i = 0.f; i < (float)16 * PI; i += 0.01) {	//	������ ������ ������ �׷��� ����
			glBegin(GL_POINTS);
			glVertex3f(SIN_MAX * cos(i) + i * 10, SIN_MAX * sin(i), 0.f);
			glEnd();
		}
		glPushMatrix();
			glTranslated(myfigure.x, myfigure.y, 0.f);
			//glRotatef(myfigure.radian / PI * 180.f, 0.f, 0.f, 1.f);
			if (myfigure.dirVector.y >= 0) {
				glRotatef(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI, 0.f, 0.f, 1.f);
			}
			else {
				glRotatef(-(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI), 0.f, 0.f, 1.f);
			}
			drawMyFigure();
		glPopMatrix();

	glPopMatrix();
}

void drawGraphZigzag(void)
{
	glPushMatrix();
		glTranslatef(-300.f, 0.f, 0.f);				//	������� �׷��� ������ ���� ��ǥ (-300, 0)
		glColor3f(1.f, 1.f, 1.f);
		glBegin(GL_LINE_STRIP);

		for (float i = 0; i < 16; i += 2) {						//	������ ������ ������� �׷��� ����
			glVertex3f(i * 40, 200.f, 0.f);
			glVertex3f((i + (float)1) * 40, -200.f, 0.f);
		}
		glEnd();
		
		glPushMatrix();
			glTranslated(myfigure.x, myfigure.y, 0.f);
			if (myfigure.dirVector.y >= 0) {
				glRotatef(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI, 0.f, 0.f, 1.f);
			}
			else {
				glRotatef(-(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI), 0.f, 0.f, 1.f);
			}
			drawMyFigure();
		glPopMatrix();
	glPopMatrix();
}

void drawGraphFreeDraw(void)
{
	glColor3f(1.f, 0.f, 0.f);
	glPointSize(5.f);
	for (int i = 0; i < 5; ++i) {								//	�÷��̾ ���� ���� ���� ǥ��
		glBegin(GL_POINTS);
		if (freedrawArray[i].isGenerated) glVertex3f(freedrawArray[i].x, freedrawArray[i].y, 0.f);
		glEnd();
	}
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINE_STRIP);										//	�÷��׿� ���� ���� �� ���� ���� �׷��� ����
	if (freedrawArray[0].isGenerated) glVertex3f(freedrawArray[0].x, freedrawArray[0].y, 0.f);
	if (freedrawArray[1].isGenerated) glVertex3f(freedrawArray[1].x, freedrawArray[1].y, 0.f);
	if (freedrawArray[2].isGenerated) glVertex3f(freedrawArray[2].x, freedrawArray[2].y, 0.f);
	if (freedrawArray[3].isGenerated) glVertex3f(freedrawArray[3].x, freedrawArray[3].y, 0.f);
	if (freedrawArray[4].isGenerated) glVertex3f(freedrawArray[4].x, freedrawArray[4].y, 0.f);
	glEnd();

	glPushMatrix();
		glTranslated(myfigure.x, myfigure.y, 0.f);
		if (myfigure.dirVector.y >= 0) {
			glRotatef(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI, 0.f, 0.f, 1.f);
		}
		else {
			glRotatef(-(acos(dotproduct(myfigure.dirVector, xAxis))*180.f / PI), 0.f, 0.f, 1.f);
		}
		drawMyFigure();
	glPopMatrix();
}

void drawTriangle(void)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(MYFIGURE_SIZE*sqrtf(3.f),	0,				0.f);
	glVertex3f(0,							-MYFIGURE_SIZE, 0.f);
	glVertex3f(0,							MYFIGURE_SIZE, 0.f);
	glEnd();
}

void drawRectangle(void)
{
	glBegin(GL_POLYGON);
	glVertex3f(	MYFIGURE_SIZE,	MYFIGURE_SIZE, 0.f);
	glVertex3f(-MYFIGURE_SIZE,	MYFIGURE_SIZE, 0.f);
	glVertex3f(-MYFIGURE_SIZE, -MYFIGURE_SIZE, 0.f);
	glVertex3f(	MYFIGURE_SIZE, -MYFIGURE_SIZE, 0.f);
	glEnd();
}

void drawMyFigure(void)
{
	glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
	if (myfigure.shape		== RECTANGLE) {
		drawRectangle();
	}
	else if (myfigure.shape == TRIANGLE) {
		drawTriangle();
	}
}
