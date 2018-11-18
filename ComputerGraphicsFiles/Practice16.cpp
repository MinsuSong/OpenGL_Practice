
#include <gl/freeglut.h>
#include "myHeader.h"

GLvoid drawAxis();
GLvoid drawGround();
GLvoid rotateScreen();
GLvoid drawShape();
GLvoid drawLeftShape();
GLvoid drawRightShape();

bool isStarted = false;
GLint counterRotateY{ 0 };
GLint counterRotatey{ 0 };
GLint counterRotatex{ 0 };
GLint counterRotateX{ 0 };
GLint counterRotatez{ 0 };
GLint counterRotateZ{ 0 };

GLint leftRotate{ 0 };
GLint rightRotate{ 0 };
GLint y_AxisShapeRotate{ 0 };

#define CONE	1
#define CUBE	2
#define SPHERE	3
#define TORUS	4
#define TEAPOT	5



struct SHAPEs {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	GLfloat size{ 0 };
	GLint shape{ CONE };
};

SHAPEs left;	//	Solid Model
SHAPEs right;	//	Wire Model

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice16");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	//	Call Back Fuctions
	glutTimerFunc(100, timerfunction, 1);			//	Timer
	glutMouseFunc(mousemovefunction);				//	Mouse
	glutKeyboardFunc(keydownfunction);				//	Keyboard

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);


	glPushMatrix();
	
	//	행렬 연산 누적
	/* 축 회전에 대한 기본 누적 */
	rotateScreen();

	//	누적 연산 pop해서, 축 고정 시켜줄려고.
	drawGround();
	drawAxis();
	drawShape();

	glPopMatrix();

	
	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid Reshape(int w, int h)
{
	/* 화면 좌표계 설정 */
	glViewport(0, 0, w, h);

	/* ViewprojectionMatrix 설정하겠음 */
	glMatrixMode(GL_PROJECTION);
	/* 동차좌표 행렬을 단위 행렬로 초기화 */
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), 1.0, 10000.0);

	gluLookAt(
		0.0, 30.0, 250.0,
		0.0, 30.0, 249.0,
		0.0, 1.0, 0.0);
	
	/* 모델좌표계로 돌아가 월드좌표 변환 계속 */
	glMatrixMode(GL_MODELVIEW);
	/* 변환 행렬 단위 행렬로 초기화 */
	glLoadIdentity();
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
		/* 도형 선택 */
	case '1':
		left.shape = CONE;
		right.shape = CONE;
		break;
	case '2':
		left.shape = CUBE;
		right.shape = CUBE;
		break;
	case '3':
		left.shape = SPHERE;
		right.shape = SPHERE;
		break;
	case '4':
		left.shape = TORUS;
		right.shape = TORUS;
		break;
	case '5':
		left.shape = TEAPOT;
		right.shape = TEAPOT;
		break;

		/* y축에 대한 회전 */
	case 'y':	//	음의 이동
		counterRotatey++;
		break;
	case 'Y':	//	양의 이동
		counterRotateY++;
		break;
		/* z축에 대한 회전 */
	case 'z':	//	음의 이동
		counterRotatez++;
		break;
	case 'Z':	//	양의 이동
		counterRotateZ++;
		break;
		/* x축에 대한 회전 */
	case 'x':	//	음의 이동
		counterRotatex++;
		break;
	case 'X':	//	양의 이동
		counterRotateX++;
		break;

		/* 도형 회전 커맨드 */
	case 'L':
	case 'l':
		leftRotate++;
		break;
	case 'R':
	case 'r':
		rightRotate++;
		break;
	case 'O':
	case 'o':
		y_AxisShapeRotate++;
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
	printf("%cKey is pressed\n", key);
	glutPostRedisplay();
}

GLvoid drawAxis(void)
{
	glBegin(GL_LINES);	//	Y 축
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -100.f, 0.f);
	glVertex3f(0.f,	100.f,  0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z 축
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, -100.f);
	glVertex3f(0.f, 0.f, 100.f);
	glEnd();
	glBegin(GL_LINES);	//	X 축
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-100.f, 0.f, 0.f);
	glVertex3f(	100.f, 0.f, 0.f);
	glEnd();
}

GLvoid drawGround()
{
	glPushMatrix();
		glColor3f(1.f, 1.f, 1.f);
		glScalef(GROUND_SIZE, 1, GROUND_SIZE);
		glutSolidCube(1);
	glPopMatrix();
}

GLvoid rotateScreen()
{
	/* y축 회전 */
	for (int i = 0; i < counterRotateY; ++i) {
		glRotatef(10.f,		0.f,	1.f,	0.f);
	}for (int i = 0; i < counterRotatey; ++i) {
		glRotatef(-10.f,	0.f,	1.f,	0.f);
	}
	/* z축 회전 */
	for (int i = 0; i < counterRotateZ; ++i) {
		glRotatef(10.f,		0.f,	0.f,	1.f);
	}for (int i = 0; i < counterRotatez; ++i) {
		glRotatef(-10.f,	0.f,	0.f,	1.f);
	}
	/* x축 회전 */
	for (int i = 0; i < counterRotateX; ++i) {
		glRotatef(10.f,		1.f,	0.f,	0.f);
	}for (int i = 0; i < counterRotatex; ++i) {
		glRotatef(-10.f,	1.f,	0.f,	0.f);
	}
}

GLvoid drawShape()
{
	drawLeftShape();
	drawRightShape();
}

GLvoid drawLeftShape()
{
	glPushMatrix();
	glColor3f(0.f, 0.75f, 0.f);


	for (int i = 0; i < y_AxisShapeRotate; ++i) {
		glRotatef(5.f, 0.f, 1.f, 0.f);
	}
	glTranslatef(-100.f, 0.f, 0.f);
	for (int i = 0; i < leftRotate; ++i) {
		glRotatef(5.f, 0.f, 1.f, 0.f);
	}
	if (left.shape != TEAPOT) {
		glPushMatrix();
		glRotatef(-90, 1.f, 0.f, 0.f);
	}
	if (left.shape == CONE) {
		glutSolidCone(10.0, 10.0, 10, 10);
		glPopMatrix();
	}
	else if (left.shape == CUBE) {
		glutSolidCube(10.0);
		glPopMatrix();
	}
	else if (left.shape == SPHERE) {
		glutSolidSphere(10.0, 10, 10);
		glPopMatrix();
	}
	else if (left.shape == TORUS) {
		glutSolidTorus(5.0, 10.0, 10, 10);
		glPopMatrix();
	}
	else if (left.shape == TEAPOT) {
		glutSolidTeapot(10.0);
	}
	glPopMatrix();
}

GLvoid drawRightShape()
{
	glPushMatrix();
	glColor3f(1.f, 0.75f, 0.f);
	for (int i = 0; i < y_AxisShapeRotate; ++i) {
		glRotatef(5.f, 0.f, 1.f, 0.f);
	}
	glTranslatef(100.f,0.f,0.f);
	for (int i = 0; i < rightRotate; ++i) {
		glRotatef(5.f, 0.f, 1.f, 0.f);
	}
	if (right.shape != TEAPOT) {
		glPushMatrix();
		glRotatef(-90, 1.f, 0.f, 0.f);
	}
	if (right.shape == CONE) {
		glutWireCone(10.0, 10.0, 10, 10);
		glPopMatrix();
	}
	else if (right.shape == CUBE) {
		glutWireCube(10.0);
		glPopMatrix();
	}
	else if (right.shape == SPHERE) {
		glutWireSphere(10.0, 10, 10);
		glPopMatrix();
	}
	else if (right.shape == TORUS) {
		glutWireTorus(5.0, 10.0, 10, 10);
		glPopMatrix();
	}
	else if (right.shape == TEAPOT) {
		glutWireTeapot(10.0);
	}
	glPopMatrix();
}
