
#include <gl/freeglut.h>
#include "myHeader.h"

GLvoid drawAxis();
GLvoid drawGround();
GLvoid convertScreen();
GLvoid drawShape();
GLvoid drawLeftShape();
GLvoid drawRightShape();

GLvoid matrixProductX(Vector* v);
GLvoid matrixProductx(Vector* v);
GLvoid matrixProductY(Vector* v);
GLvoid matrixProducty(Vector* v);
GLvoid matrixProductZ(Vector* v);
GLvoid matrixProductz(Vector* v);

bool isStarted = false;

GLfloat leftRotate{ 0 };
GLfloat rightRotate{ 0 };

#define CONE	1
#define CUBE	2
#define SPHERE	3
#define TORUS	4
#define TEAPOT	5

//GLfloat 

Vector eye{ 0.f,30.f,250.f };

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
	glutCreateWindow("Practice18");
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

	//	��� ���� ����
	/* �� ȸ���� ���� �⺻ ���� */
	convertScreen();

	//	���� ���� pop�ؼ�, �� ���� �����ٷ���.
	drawGround();
	drawAxis();
	drawShape();

	glPopMatrix();


	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid Reshape(int w, int h)
{
	/* ȭ�� ��ǥ�� ���� */
	glViewport(0, 0, w, h);

	/* ViewprojectionMatrix �����ϰ��� */
	glMatrixMode(GL_PROJECTION);
	/* ������ǥ ����� ���� ��ķ� �ʱ�ȭ */
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), 1.0, 10000.0);

	gluLookAt(
		eye.x, eye.y, eye.z,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

	/* ����ǥ��� ���ư� ������ǥ ��ȯ ��� */
	glMatrixMode(GL_MODELVIEW);
	/* ��ȯ ��� ���� ��ķ� �ʱ�ȭ */
	glLoadIdentity();
}

void timerfunction(int value)
{
	if (leftRotate > 2 * PI) {
		leftRotate = 0.f;
	}
	else {
		leftRotate += 0.05f;
	}
	if (rightRotate > 2 * PI) {
		rightRotate = 0.f;
	}
	else {
		rightRotate += 0.05f;
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
		/* ���� ���� */
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

		/* 5���� �� 0.0872665 rad */
		/* y�࿡ ���� ȸ�� */
	case 'y':	//	���� �̵�
		matrixProducty(&eye);
		break;
	case 'Y':	//	���� �̵�
		matrixProductY(&eye);
		break;
		/* z�࿡ ���� ȸ�� */
	case 'z':	//	���� �̵�
		matrixProductz(&eye);
		break;
	case 'Z':	//	���� �̵�
		matrixProductZ(&eye);
		break;
		/* x�࿡ ���� ȸ�� */
	case 'x':	//	���� �̵�
		matrixProductx(&eye);
		break;
	case 'X':	//	���� �̵�
		matrixProductX(&eye);
		break;

	case 'i':
		eye.x = 0.f;
		eye.y = 30.f;
		eye.z = 250.f;
		break;

	case 'w':
		eye.y += 5;
		break;
	case 'a':
		eye.x -= 5;
		break;
	case 's':
		eye.y -= 5;
		break;
	case 'd':
		eye.x += 5;
		break;

	case '+':
	{
		Vector tmp;
		tmp.x = eye.x;
		tmp.y = eye.y;
		tmp.z = eye.z;
		normalize(&tmp);
		eye.x += tmp.x * 10;
		eye.y += tmp.y * 10;
		eye.z += tmp.z * 10;
	}
		break;
	case '-':
	{
		Vector tmp;
		tmp.x = eye.x;
		tmp.y = eye.y;
		tmp.z = eye.z;
		normalize(&tmp);
		eye.x -= tmp.x * 10;
		eye.y -= tmp.y * 10;
		eye.z -= tmp.z * 10;
	}
		break;

		/* ���α׷� ���� Ŀ�ǵ� */
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
	Reshape(800, 600);
	glutPostRedisplay();
}

GLvoid drawAxis(void)
{
	glLineWidth(5.f);
	glBegin(GL_LINES);	//	Y ��
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -100.f, 0.f);
	glVertex3f(0.f, 100.f, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z ��
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, -100.f);
	glVertex3f(0.f, 0.f, 100.f);
	glEnd();
	glBegin(GL_LINES);	//	X ��
	glColor3f(1.0f, 0.f, 0.f);
	glVertex3f(-100.f, 0.f, 0.f);
	glVertex3f(100.f, 0.f, 0.f);
	glEnd();
	glLineWidth(1.f);
}

GLvoid drawGround()
{
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, -1.f, 0.f);
	glScalef(GROUND_SIZE, 1, GROUND_SIZE);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid convertScreen()
{

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
	glTranslatef(-50.f, 0.f, 0.f);
	glRotatef(-(leftRotate * 180 / PI), 0.f, 1.f, 0.f);
	if (left.shape != CONE) {
		glPushMatrix();
		glTranslatef(0.f, 25.f, 0.f);
	}
	if (left.shape != TEAPOT) {
		glPushMatrix();
		glRotatef(-90, 1.f, 0.f, 0.f);
	}
	if (left.shape == CONE) {
		glutSolidCone(25.0, 25.0, 10, 10);
		glPopMatrix();
	}
	else if (left.shape == CUBE) {
		glutSolidCube(25.0);
		glPopMatrix();
		glPopMatrix();
	}
	else if (left.shape == SPHERE) {
		glutSolidSphere(25.0, 10, 10);
		glPopMatrix();
		glPopMatrix();
	}
	else if (left.shape == TORUS) {
		glutSolidTorus(12.5, 25.0, 10, 10);
		glPopMatrix();
		glPopMatrix();
	}
	else if (left.shape == TEAPOT) {
		glutSolidTeapot(25.0);
		glPopMatrix();
	}
	glPopMatrix();
}

GLvoid drawRightShape()
{
	glPushMatrix();
	glColor3f(1.f, 0.75f, 0.f);
	glTranslatef(50.f, 0.f, 0.f);
	glRotatef(rightRotate * 180 / PI, 0.f, 1.f, 0.f);
	
	if (right.shape != CONE) {
		glPushMatrix();
		glTranslatef(0.f, 25.f, 0.f);
	}
	if (right.shape != TEAPOT) {
		glPushMatrix();
		glRotatef(-90, 1.f, 0.f, 0.f);
	}
	if (right.shape == CONE) {
		glutWireCone(25.0, 25.0, 10, 10);
		glPopMatrix();
	}
	else if (right.shape == CUBE) {
		glutWireCube(25.0);
		glPopMatrix();
		glPopMatrix();
	}
	else if (right.shape == SPHERE) {
		glutWireSphere(25.0, 10, 10);
		glPopMatrix();
		glPopMatrix();
	}
	else if (right.shape == TORUS) {
		glutWireTorus(12.5, 25.0, 10, 10);
		glPopMatrix();
		glPopMatrix();
	}
	else if (right.shape == TEAPOT) {
		glutWireTeapot(25.0);
		glPopMatrix();
	}
	glPopMatrix();
}

GLvoid matrixProductX(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->y = cos(0.087)* old.y - sin(0.087)* old.z;
	v->z = sin(0.087)* old.y + cos(0.087)* old.z;
}

GLvoid matrixProductY(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->x = cos(0.087)* old.x + sin(0.087)* old.z;
	v->z = -sin(0.087)* old.x + cos(0.087)* old.z;
}

GLvoid matrixProductZ(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->x = cos(0.087)* old.x - sin(0.087)* old.y;
	v->y = sin(0.087)* old.x + cos(0.087)* old.y;
}

GLvoid matrixProductx(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->y = cos(-0.087)* old.y - sin(-0.087)* old.z;
	v->z = sin(-0.087)* old.y + cos(-0.087)* old.z;
}

GLvoid matrixProducty(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->x = cos(-0.087)* old.x + sin(-0.087)* old.z;
	v->z = -sin(-0.087)* old.x + cos(-0.087)* old.z;
}

GLvoid matrixProductz(Vector * v)
{
	Vector old;
	old.x = v->x;
	old.y = v->y;
	old.z = v->z;
	v->x = cos(-0.087)* old.x - sin(-0.087)* old.y;
	v->y = sin(-0.087)* old.x + cos(-0.087)* old.y;
}
