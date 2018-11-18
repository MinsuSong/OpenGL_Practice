#include <gl/freeglut.h>
#include "CGMath.h"
#include "GlobalDefines.h"
#include <timeapi.h>

DWORD g_prevTime = 0;

#define BODY_H	10.f 
#define LARM_H	50.f
#define UARM_H	45.f

#define NONE	0
#define RIGHT	1
#define TOP		-1
#define LEFT	-1
#define BOTTOM	1

GLvoid drawAxis();
GLvoid drawGround();
GLvoid convertScreen();
GLvoid drawWorld();
GLvoid drawBody();
GLvoid drawLowerArm();
GLvoid drawUpperArm();
GLvoid drawRunningMachine();
GLvoid drawPullupBar();
GLvoid drawBenchPress();
GLvoid drawTree();
GLvoid drawSphere();
GLvoid drawSector();
GLvoid drawLTSector();
GLvoid drawRTSector();
GLvoid drawLBSector();
GLvoid drawRBSector();

GLvoid matrixProductX(myVector4f* v);
GLvoid matrixProductx(myVector4f* v);
GLvoid matrixProductY(myVector4f* v);
GLvoid matrixProducty(myVector4f* v);
GLvoid matrixProductZ(myVector4f* v);
GLvoid matrixProductz(myVector4f* v);

myVector4f eye{ 0.f,150.f,250.f };

struct SHAPEs {
	myVector4f pos;
	myVector4f vel;
	GLfloat size{ 0.f };
	GLfloat Xangle{ 0.f };
	GLfloat Zangle{ 0.f };
};

SHAPEs myCrane;
SHAPEs mySphere;

GLfloat BODY_YAngle = 0.f;
GLfloat LARM_XAngle = 0.f;
GLfloat LARM_YAngle = 0.f;
GLfloat UARM_XAngle = 0.f;
GLfloat UARM_ZAngle = 0.f;

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice21");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutIdleFunc(CB_idle);							//	idle
	glEnable(GL_DEPTH_TEST);



	glutMainLoop();
}

GLvoid RenderScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	/* 프레임 시간 계산 */
	if (g_prevTime == 0) {
		g_prevTime = timeGetTime();
		return;
	}
	DWORD currTime = timeGetTime();
	DWORD elapsedTime = currTime - g_prevTime;
	g_prevTime = currTime;
	float eTime = static_cast<float>(elapsedTime) / 1000.f;

	glPushMatrix();

	update(eTime);

	drawGround();
	drawSector();
	drawAxis();
	drawWorld();

	glPopMatrix();


	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid ReShape(int w, int h)
{
	/* 화면 좌표계 설정 */
	glViewport(0, 0, w, h);

	/* ViewprojectionMatrix 설정하겠음 */
	glMatrixMode(GL_PROJECTION);
	/* 동차좌표 행렬을 단위 행렬로 초기화 */
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLdouble>(w) / static_cast<GLdouble>(h), 1.0, 10000.0);

	gluLookAt(
		eye.x, eye.y, eye.z,
		0.0, 0.0, -1.0,
		0.0, 1.0, 0.0);

	/* 모델좌표계로 돌아가 월드좌표 변환 계속 */
	glMatrixMode(GL_MODELVIEW);
	/* 변환 행렬 단위 행렬로 초기화 */
	glLoadIdentity();
}

void CB_idle()
{
	RenderScene();
}

void update(float eTime)
{


	RenderScene();
}

void CB_mousemovefunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}
}

void CB_keydownfunc(unsigned char key, int x, int y)
{
	switch (key)
	{
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

	case '+':
	{
		myVector4f tmp = eye;
		tmp = normalize(tmp);
		eye.x += tmp.x * 10;
		eye.y += tmp.y * 10;
		eye.z += tmp.z * 10;
	}
	break;
	case '-':
	{
		myVector4f tmp = eye;
		tmp = normalize(tmp);
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
		break;


	case 'a':
		eye.x -= 5;
		break;
	case 'w':
		eye.y += 5;
		break;
	case 'd':
		eye.x += 5;
		break;
	case 's':
		eye.y -= 5;
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();

	case 'i':
	case 'I':
	{
		/* 카메라 초기화 */
		eye.x = 0.f;
		eye.y = 30.f;
		eye.z = 250.f;
	}
	break;

	/* upper */
	case 'f':	//	z축
		if (UARM_ZAngle < 90.f) {
			UARM_ZAngle += 5.f;
		}
		break;
	case 'F':
		if (UARM_ZAngle > -90.f) {
			UARM_ZAngle -= 5.f;
		}
		break;
	case 'g':	//	x축
		if (UARM_XAngle < 90.f) {
			UARM_XAngle += 5.f;
		}
		break;
	case 'G':
		if (UARM_XAngle > -90.f) {
			UARM_XAngle -= 5.f;
		}
		break;

		/* lower */
	case 'h':	//	y축
		LARM_YAngle += 5.f;
		break;
	case 'H':
		LARM_YAngle -= 5.f;
		break;
	case 'j':	//	x축
		if (LARM_XAngle < 90.f) {
			LARM_XAngle += 5.f;
		}
		break;
	case 'J':
		if (LARM_XAngle > -90.f) {
			LARM_XAngle -= 5.f;
		}
		break;
		/**/
	case 'n':
		BODY_YAngle += 5.f;
		break;
	case 'N':
		BODY_YAngle -= 5.f;
		break;

	case 'c':
		break;

	case 'v':
		break;

	default:
		break;
	}
	printf("%cKey is pressed\n", key);
	ReShape(800, 600);
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
	glColor3f(0.7f, 0.7f, 0.7f);
	glTranslatef(0.f, -1.f, 0.f);
	glScalef(DEFAULT_GROUND_SIZE, 1, DEFAULT_GROUND_SIZE);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid drawWorld()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(myCrane.pos.x, 0.f, 0.f);
	glTranslatef(0.f, BODY_H / 2.0f, 0.f);
	glRotatef(BODY_YAngle, 0.f, 1.f, 0.f);
	drawBody();

	glPushMatrix();
	glTranslatef(0.f, BODY_H / 2.f, 0.f);
	glRotatef(LARM_XAngle, 1.f, 0.f, 0.f);
	glRotatef(LARM_YAngle, 0.f, 1.f, 0.f);
	glTranslatef(0.f, LARM_H / 2.f, 0.f);
	drawLowerArm();

	glPushMatrix();
	glTranslatef(0.f, LARM_H / 2.f, 0.f);
	glRotatef(UARM_XAngle, 1.f, 0.f, 0.f);
	glRotatef(UARM_ZAngle, 0.f, 0.f, 1.f);
	glTranslatef(0.f, UARM_H / 2.f, 0.f);
	drawUpperArm();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(mySphere.pos.x * PI / 9.f, 0.f, 0.f);
	drawSphere();
	glPopMatrix();
}

GLvoid drawBody()
{
	/* red body */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glScalef(50.f, BODY_H, 25.f);
	glutWireCube(1);
	glColor3f(0.7f, 0.1f, 0.1f);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid drawLowerArm()
{
	/* height = 50 */
	/* green lower arm */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glScalef(10.f, LARM_H, 10.f);
	glutWireCube(1);
	glColor3f(0.1f, 0.6f, 0.1f);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid drawUpperArm()
{
	/* height = 45 */
	/* teal upper arm */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glScalef(8.f, UARM_H, 8.f);
	glutWireCube(1);
	glColor3f(0.1f, 0.6f, 0.6f);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid matrixProductX(myVector4f * v)
{
	myVector4f old = *v;
	v->y = cos(0.087)* old.y - sin(0.087)* old.z;
	v->z = sin(0.087)* old.y + cos(0.087)* old.z;
}

GLvoid matrixProductY(myVector4f * v)
{
	myVector4f old = *v;
	v->x = cos(0.087)* old.x + sin(0.087)* old.z;
	v->z = -sin(0.087)* old.x + cos(0.087)* old.z;
}

GLvoid matrixProductZ(myVector4f * v)
{
	myVector4f old = *v;
	v->x = cos(0.087)* old.x - sin(0.087)* old.y;
	v->y = sin(0.087)* old.x + cos(0.087)* old.y;
}

GLvoid matrixProductx(myVector4f * v)
{
	myVector4f old = *v;
	v->y = cos(-0.087)* old.y - sin(-0.087)* old.z;
	v->z = sin(-0.087)* old.y + cos(-0.087)* old.z;
}

GLvoid matrixProducty(myVector4f * v)
{
	myVector4f old = *v;
	v->x = cos(-0.087)* old.x + sin(-0.087)* old.z;
	v->z = -sin(-0.087)* old.x + cos(-0.087)* old.z;
}

GLvoid matrixProductz(myVector4f * v)
{
	myVector4f old = *v;
	v->x = cos(-0.087)* old.x - sin(-0.087)* old.y;
	v->y = sin(-0.087)* old.x + cos(-0.087)* old.y;
}

GLvoid drawSphere()
{
	/* radius = 20 */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, 20.f, 0.f);
	glRotatef(-mySphere.Xangle, 0.f, 0.f, 1.f);
	glRotatef(mySphere.Zangle, 1.f, 0.f, 0.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glutWireSphere(20.f, 10, 10);
	glColor3f(0.1f, 0.3f, 0.1f);
	glutSolidSphere(19.f, 20, 20);
	glPopMatrix();
}

GLvoid drawSector()
{
	drawLTSector();
	drawLBSector();
	drawRBSector();
	drawRTSector();
}

GLvoid drawLTSector()
{
	glPushMatrix();
	glTranslatef(-150.f, 0.f, -150.f);
	glColor3f(0.5f, 0.f, 0.f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 150.f);
	glVertex3f(150.f, 0.f, 0.f);
	glEnd();
	glPopMatrix();
}

GLvoid drawRTSector()
{
	glPushMatrix();
	glTranslatef(150.f, 0.f, -150.f);
	glColor3f(0.5f, 0.25f, 0.f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(-150.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 150.f);
	glEnd();
	glPopMatrix();
}

GLvoid drawLBSector()
{
	glPushMatrix();
	glTranslatef(-150.f, 0.f, 150.f);
	glColor3f(0.f, 0.25f, 0.25f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(150.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, -150.f);
	glEnd();
	glPopMatrix();
}

GLvoid drawRBSector()
{
	glPushMatrix();
	glTranslatef(150.f, 0.f, 150.f);
	glColor3f(0.f, 0.5f, 0.75f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, -150.f);
	glVertex3f(-150.f, 0.f, 0.f);
	glEnd();
	glPopMatrix();
}
