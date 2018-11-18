
#include <gl/freeglut.h>
#include "myHeader.h"

#define WIRE_MODEL	100
#define SOLID_MODEL	101

bool isPerspective{ true };

struct COLOR {
	GLfloat r = 0;
	GLfloat g = 0;
	GLfloat b = 0;
};

struct SHAPEs {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	GLfloat radius{ 0 };
	COLOR rgb;
	GLint shape{ SOLID_MODEL };
};

GLvoid Initiate();

GLvoid drawAxis();
GLvoid drawOrbit();
GLvoid drawSmallOrbit();
GLvoid drawSpecialOrbit();
GLvoid drawSphere(SHAPEs * star);

GLvoid matrixProductX(Vector* v);
GLvoid matrixProductx(Vector* v);
GLvoid matrixProductY(Vector* v);
GLvoid matrixProducty(Vector* v);
GLvoid matrixProductZ(Vector* v);
GLvoid matrixProductz(Vector* v);

GLfloat leftRotate{ 0 };
GLfloat rightRotate{ 0 };

Vector eye{ 0.f,30.f,250.f };

SHAPEs Sun;
SHAPEs Earth[3];
SHAPEs Moon[3];

GLfloat FastOrbitRad{ 0.f };
GLfloat MediumOrbitRad{ 0.f };
GLfloat SlowOrbitRad{ 0.f };


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice17");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	//	Call Back Fuctions
	glutTimerFunc(100, timerfunction, 1);			//	Timer
	glutMouseFunc(mousemovefunction);				//	Mouse
	glutKeyboardFunc(keydownfunction);				//	Keyboard

	glEnable(GL_DEPTH_TEST);

	Initiate();

	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		drawSphere(&Sun);
		drawAxis();

		glPushMatrix();
			glRotatef(90, 1.f, 0.f, 0.f);
			//drawOrbit();
			drawSpecialOrbit();
			glTranslatef(150.f * cos(FastOrbitRad), 90.f * sin(FastOrbitRad), 0.f);
			drawSphere(&Earth[0]);

			glPushMatrix();
				drawSmallOrbit();
				glTranslatef(10.f * cos(SlowOrbitRad), 10.f * sin(SlowOrbitRad), 0.f);
				drawSphere(&Moon[0]);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glRotatef(45, 0.f, 0.f, 1.f);
			glRotatef(90, 1.f, 0.f, 0.f);
			drawOrbit();
			glTranslatef(100.f * cos(MediumOrbitRad), 100.f * sin(MediumOrbitRad), 0.f);
			drawSphere(&Earth[1]);

			glPushMatrix();
				drawSmallOrbit();
				glTranslatef(10.f * cos(MediumOrbitRad), 10.f * sin(MediumOrbitRad), 0.f);
				drawSphere(&Moon[1]);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			glRotatef(-45, 0.f, 0.f, 1.f);
			glRotatef(90, 1.f, 0.f, 0.f);
			drawOrbit();
			glTranslatef(100.f * cos(SlowOrbitRad), 100.f * sin(SlowOrbitRad), 0.f);
			drawSphere(&Earth[2]);

			glPushMatrix();
				drawSmallOrbit();
				glTranslatef(10.f * cos(FastOrbitRad), 10.f * sin(FastOrbitRad), 0.f);
				drawSphere(&Moon[2]);
			glPopMatrix();
		glPopMatrix();

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
	if (isPerspective) {
		gluPerspective(60.0, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), 1.0, 10000.0);
	}
	else {
		glOrtho(-400.0, 400.0, -300.0, 300.0, -400.0, 400.0);
	}
	gluLookAt(
		eye.x, eye.y, eye.z,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

	/* 모델좌표계로 돌아가 월드좌표 변환 계속 */
	glMatrixMode(GL_MODELVIEW);
	/* 변환 행렬 단위 행렬로 초기화 */
	glLoadIdentity();
}

void timerfunction(int value)
{
	FastOrbitRad += 0.25;
	MediumOrbitRad += 0.05;
	SlowOrbitRad += 0.01;

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
	case '1':	//	Solid
		Sun.shape = SOLID_MODEL;
		Earth[0].shape = SOLID_MODEL;
		Earth[1].shape = SOLID_MODEL;
		Earth[2].shape = SOLID_MODEL;
		Moon[0].shape = SOLID_MODEL;
		Moon[1].shape = SOLID_MODEL;
		Moon[2].shape = SOLID_MODEL;
		break;
	case '2':	//	Wire
		Sun.shape = WIRE_MODEL;
		Earth[0].shape = WIRE_MODEL;
		Earth[1].shape = WIRE_MODEL;
		Earth[2].shape = WIRE_MODEL;
		Moon[0].shape = WIRE_MODEL;
		Moon[1].shape = WIRE_MODEL;
		Moon[2].shape = WIRE_MODEL;
		break;

		/* 5도는 약 0.0872665 rad */
		/* y축에 대한 회전 */
	case 'y':	//	양의 이동
		matrixProducty(&eye);
		break;
	case 'Y':	//	음의 이동
		matrixProductY(&eye);
		break;
		/* z축에 대한 회전 */
	case 'z':	//	양의 이동
		matrixProductz(&eye);
		break;
	case 'Z':	//	음의 이동
		matrixProductZ(&eye);
		break;
		/* x축에 대한 회전 */
	case 'x':	//	양의 이동
		matrixProductx(&eye);
		break;
	case 'X':	//	음의 이동
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

	/* 프로그램 종료 커맨드 */
	case VK_ESCAPE:	//	ESC
		PostQuitMessage(0);
	case VK_RETURN:	//	Enter
		break;
	case VK_BACK:	//	Back space
		break;
	case VK_SPACE:	//	space bar
		if (isPerspective) {
			isPerspective = false;
		}
		else {
			isPerspective = true;
		}
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
	glBegin(GL_LINES);	//	Y 축
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, -100.f, 0.f);
	glVertex3f(0.f, 100.f, 0.f);
	glEnd();
	glBegin(GL_LINES);	//	Z 축
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, -100.f);
	glVertex3f(0.f, 0.f, 100.f);
	glEnd();
	glBegin(GL_LINES);	//	X 축
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

GLvoid Initiate()
{
	{
		Sun.radius = 30;
		Sun.rgb.r = 0.75f;
	}
	{
		Earth[0].rgb.b = 0.75f;
		Earth[0].radius = 7.5f;
		Earth[1].rgb.r = 0.25f;
		Earth[1].rgb.b = 0.75f;
		Earth[1].radius = 8.75f;
		Earth[2].rgb.g = 1.0f;
		Earth[2].rgb.b = 0.75f;
		Earth[2].radius = 4.5f;
	}
	{
		Moon[0].rgb.r = 0.5f;
		Moon[0].rgb.g = 0.9f;
		Moon[0].rgb.b = 0.5f;
		Moon[0].radius = 2.5f;
		Moon[1].rgb.r = 0.25f;
		Moon[1].rgb.g = 0.5f;
		Moon[1].rgb.b = 0.5f;
		Moon[1].radius = 2.5f;
		Moon[2].rgb.r = 0.75f;
		Moon[2].rgb.g = 0.35f;
		Moon[2].rgb.b = 0.25f;
		Moon[2].radius = 5.5f;
	}
}

GLvoid drawOrbit()
{
	glPointSize(1.f);
	glColor3f(1.f, 1.f, 1.f);
	for (float i = 0.f; i < 2 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex3f(100.f * cos(i), (float)100.f * sin(i), 0.f);
		glEnd();
	}
}

GLvoid drawSmallOrbit()
{
	glPointSize(1.f);
	glColor3f(1.f, 1.f, 1.f);
	for (float i = 0.f; i < 2 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex3f(10.f * cos(i), (float)10.f * sin(i), 0.f);
		glEnd();
	}
}

GLvoid drawSpecialOrbit()
{
	glPointSize(1.f);
	glColor3f(1.f, 1.f, 1.f);
	for (float i = 0.f; i < 2 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex3f(150.f * cos(i), (float)90.f * sin(i), 0.f);
		glEnd();
	}
}

GLvoid drawSphere(SHAPEs * star)
{
	glColor3f(star->rgb.r, star->rgb.g, star->rgb.b);
	if (star->shape == SOLID_MODEL) {
		glutSolidSphere(static_cast<double>(star->radius), 10, 10);
	}
	else {
		glutWireSphere(static_cast<double>(star->radius), 10, 10);
	}
}
