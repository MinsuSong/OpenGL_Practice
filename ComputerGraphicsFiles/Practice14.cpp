#include <gl/freeglut.h>
#include "myHeader.h"

GLvoid drawAxis();
GLvoid drawFigure();

/*
Ŭ������ ��, �� �ڸ��� ����� ���� ��ɿ� ����, �װ��� xy��鿡 �������� Ŭ���Ǿ��ٰ� �����Ѵ�.
����� �Ϸ��� ���콺 ��ŷ�� �ؾ��Ѵ�.
*/

#define CONE	1
#define CUBE	2
#define SHPERE	3
#define TORUS	4
#define TEAPOT	5

bool isStarted = false;
bool isRbuttonPressed = false;
GLint counterRotateY = 0;
GLint counterRotatey = 0;

struct COLOR {
	GLfloat r{ 0 };
	GLfloat g{ 0 };
	GLfloat b{ 0 };
};

struct SHAPEs {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	GLfloat rx{ 0 };
	GLfloat ry{ 0 };
	GLfloat rz{ 0 };
	GLfloat angle{ 0 };
	GLfloat size{ 0 };
	GLint shape{ 0 };
	COLOR rgb;
	bool isGenerated{ false };
};

SHAPEs shapes[20];
GLint currentIndex{ 0 };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice14");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	//	Call Back Fuctions
	glutTimerFunc(100, timerfunction, 1);			//	Timer
	glutMouseFunc(mousemovefunction);				//	Mouse
	glutKeyboardFunc(keydownfunction);				//	Keyboard

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (!isStarted) {
		/* �� �߾����� �̵�, �� 1���� ���� */
		glTranslatef(	MAX_WINDOW_X / 2.f, MAX_WINDOW_Y / 2.f, 0.f);
		isStarted = true;
	}

	glPushMatrix();

	//	��� ���� ����
	/* y�� ȸ���� ���� �⺻ ���� */
	for (int i = 0; i < counterRotateY; ++i) {
		glRotatef(10.f, 0.f, 1.f, 0.f);
	}for (int i = 0; i < counterRotatey; ++i) {
		glRotatef(-10.f, 0.f, 1.f, 0.f);
	}

	drawFigure();


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
	for (int i = 0; i < 20; ++i) {
		if (shapes[i].isGenerated) {
			if (shapes[i].size < FLT_EPSILON) {
				shapes[i].isGenerated = false;
			}
			else {
				shapes[i].size -= 0.5;
				shapes[i].angle += 5;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(50, timerfunction, 1);
}

void mousemovefunction(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentIndex == 20) {
			currentIndex = 0;
		}
		shapes[currentIndex].isGenerated = true;
		shapes[currentIndex].shape = rand() % 4 + 1;
		shapes[currentIndex].size = rand() % 5 + 100;
		shapes[currentIndex].x = static_cast<float>(x) - (MY_WINDOW_WIDTH / 2.f);
		shapes[currentIndex].y = MY_WINDOW_HEIGHT - static_cast<float>(y) - (MY_WINDOW_HEIGHT / 2.f);
		shapes[currentIndex].z = 0;
		shapes[currentIndex].rgb.r = static_cast<float>(rand()) / RAND_MAX;
		shapes[currentIndex].rgb.g = static_cast<float>(rand()) / RAND_MAX;
		shapes[currentIndex].rgb.b = static_cast<float>(rand()) / RAND_MAX;
		shapes[currentIndex].rx = static_cast<float>(rand()) / RAND_MAX;
		shapes[currentIndex].ry = static_cast<float>(rand()) / RAND_MAX;
		shapes[currentIndex].rz = static_cast<float>(rand()) / RAND_MAX;
		currentIndex++;
	}
}

void keydownfunction(unsigned char key, int x, int y)
{
	switch (key)
	{
		/* y�࿡ ���� ȸ�� */
	case 'y':	//	���� �̵�
		counterRotatey++;
		break;
	case 'Y':	//	���� �̵�
		counterRotateY++;
		break;


	case 'r':	//	�������� ȸ�� ���� ���� ( ���� ���� �ٲ���)
	case 'R':	
		if (isRbuttonPressed) { isRbuttonPressed = false; }
		else { isRbuttonPressed = true; }
		break;

		/* ���α׷� ���� Ŀ�ǵ� */
	case VK_ESCAPE:
		PostQuitMessage(0);
	case 'q':
	case 'Q':
		glutLeaveMainLoop();

	default:
		break;
	}
	printf("%c", key);
	glutPostRedisplay();
}

GLvoid drawAxis(void)
{
	glBegin(GL_LINES);	//	Y ��
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -MAX_WINDOW_Y / 2.f, 0.f);
	glVertex3f(0.f,	MAX_WINDOW_Y / 2.f, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z ��
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, (float)MIN_WINDOW_Z);
	glVertex3f(0.f, 0.f, (float)MAX_WINDOW_Z);
	glEnd();
	glBegin(GL_LINES);	//	X ��
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-MAX_WINDOW_X / 2.f, 0.f, 0.f);
	glVertex3f(	MAX_WINDOW_X / 2.f, 0.f, 0.f);
	glEnd();
}

GLvoid drawFigure()
{
	for (int i = 0; i < 20; ++i) {
		if (shapes[i].isGenerated) {
			glPushMatrix();
			glColor3f(shapes[i].rgb.r, shapes[i].rgb.g, shapes[i].rgb.b);
			glTranslatef(shapes[i].x, shapes[i].y, shapes[i].z);
			if (isRbuttonPressed) {
				glRotatef(shapes[i].angle, shapes[i].rx, shapes[i].ry, shapes[i].rz);
			}
			else {
				glRotatef(-shapes[i].angle, shapes[i].rx, shapes[i].ry, shapes[i].rz);
			}

			if (shapes[i].shape == CONE) {
				glutWireCone(shapes[i].size, shapes[i].size, 10, 10);
			}
			else if (shapes[i].shape == CUBE) {
				glutWireCube(shapes[i].size);
			}
			else if (shapes[i].shape == SHPERE) {
				glutWireSphere(shapes[i].size, 10, 10);
			}
			else if (shapes[i].shape == TORUS) {
				glutWireTorus(shapes[i].size / 2.f, shapes[i].size, 10, 10);
			}
			else if (shapes[i].shape == TEAPOT) {
				glutWireTeapot(shapes[i].size);
			}

			glPopMatrix();
		}
	}
}
