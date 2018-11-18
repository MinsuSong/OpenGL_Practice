#include <gl/freeglut.h>
#include "myHeader.h"

GLvoid drawAxis();
GLvoid drawNumber();
GLvoid drawRect();
GLvoid drawClock();

bool isStarted = false;
GLint counterRotateY = 0;
GLint counterRotatey = 0;

struct SHAPEs {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	bool isGenerated{ false };
};

struct BlOCK {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	SHAPEs rects[7];
	GLint number{ 0 };
};

BlOCK block[6];

tm nowtime;


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice15");
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
		/* 축 중앙으로 이동, 단 1번만 연산 */
		glTranslatef(MAX_WINDOW_X / 2.f, MAX_WINDOW_Y / 2.f, 0.f);
		isStarted = true;
	}

	glPushMatrix();

	//	행렬 연산 누적
	/* y축 회전에 대한 기본 누적 */
	for (int i = 0; i < counterRotateY; ++i) {
		glRotatef(10.f, 0.f, 1.f, 0.f);
	}for (int i = 0; i < counterRotatey; ++i) {
		glRotatef(-10.f, 0.f, 1.f, 0.f);
	}

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
		/* y축에 대한 회전 */
	case 'y':	//	음의 이동
		counterRotatey++;
		break;
	case 'Y':	//	양의 이동
		counterRotateY++;
		break;

		/* 프로그램 종료 커맨드 */
	case VK_ESCAPE:	//	ESC
		PostQuitMessage(0);
	case VK_RETURN:	//	Enter
		break;
	case VK_BACK:	//	Back space
		break;
	case VK_SPACE:	//	space bar
		break;

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
	glBegin(GL_LINES);	//	Y 축
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -MAX_WINDOW_Y / 2.f, 0.f);
	glVertex3f(0.f, MAX_WINDOW_Y / 2.f, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z 축
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, (float)MIN_WINDOW_Z);
	glVertex3f(0.f, 0.f, (float)MAX_WINDOW_Z);
	glEnd();
	glBegin(GL_LINES);	//	X 축
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-MAX_WINDOW_X / 2.f, 0.f, 0.f);
	glVertex3f(MAX_WINDOW_X / 2.f, 0.f, 0.f);
	glEnd();
}

GLvoid drawRect()
{
	glColor3f(0.f, 0.f, 0.f);
	glutSolidCube(static_cast<double>(11));
	glColor3f(1.f, 1.f, 1.f);
	glutSolidCube(static_cast<double>(10));
}

GLvoid drawNumber()
{
	for (int i = 0; i < 6; ++i) {
		glPushMatrix();
		{
			if (block[i].number == 0) {
				glTranslatef(block[i].rects.x
			}
			else if (block[i].number == 1) {

			}
			else if (block[i].number == 2) {

			}
			else if (block[i].number == 3) {

			}
			else if (block[i].number == 4) {

			}
			else if (block[i].number == 5) {

			}
			else if (block[i].number == 6) {

			}
			else if (block[i].number == 7) {

			}
			else if (block[i].number == 8) {

			}
			else if (block[i].number == 9) {

			}
		}
		glPopMatrix();
	}
}
