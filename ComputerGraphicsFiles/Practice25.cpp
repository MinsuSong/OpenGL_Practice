#include <gl/freeglut.h>
#include "CGMath.h"
#include "GlobalDefines.h"
#include <timeapi.h>
#include <float.h>

DWORD g_prevTime = 0;

constexpr float CubeEdge{ 100.f };
constexpr float CubeEdgeHalf{ CubeEdge / 2.f };

constexpr float MAX_ANGLE{ 90.f };
constexpr float MIN_ANGLE{ 0.f };

constexpr float MAX_SPRING_HEIGHT{ 2.f * CubeEdge };

struct myVertex {
	myVector4f pos;
	myVector4f color;
};

void drawSphereFront(void);
void drawSphereUp(void);
void drawSpring(void);

GLvoid drawFace(const myVertex& vertex1, const myVertex& vertex2, const myVertex& vertex3, const myVertex& vertex4);

GLvoid drawAxis();
void update(float eTime);

GLvoid matrixProductX(myVector4f* v);
GLvoid matrixProductx(myVector4f* v);
GLvoid matrixProductY(myVector4f* v);
GLvoid matrixProducty(myVector4f* v);
GLvoid matrixProductZ(myVector4f* v);
GLvoid matrixProductz(myVector4f* v);

myVector4f eye{ 0.f,150.f,250.f };

myVertex vertexlist[8];

GLfloat upAngle{ 0.f };
GLfloat frontAngle{ 0.f };

bool isCullOn{ true };
bool isDepthtestOn{ true };
bool isFrontOpened{ false };
bool isTopOpened{ false };
bool isSmoothOn{ true };
GLfloat g_spring_height;
GLfloat g_frontSphereAngle{ 0.f };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice25");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutIdleFunc(CB_idle);							//	idle

	{
		myVector4f tmpPos(-CubeEdgeHalf, CubeEdgeHalf,-CubeEdgeHalf);
		myVector4f tmpColor(1.f,0.f,0.f);	//	r
		vertexlist[0].color = tmpColor;
		vertexlist[0].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-CubeEdgeHalf, CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.f, 1.f, 0.f);	//	g
		vertexlist[1].color = tmpColor;
		vertexlist[1].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.f, 0.f, 1.f);	//	b
		vertexlist[2].color = tmpColor;
		vertexlist[2].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(1.f, 1.f, 0.f);	//	y
		vertexlist[3].color = tmpColor;
		vertexlist[3].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-CubeEdgeHalf, -CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(0.f, 1.f, 1.f);	//	c
		vertexlist[4].color = tmpColor;
		vertexlist[4].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, -CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(1.f, 0.f, 1.f);	//	m
		vertexlist[5].color = tmpColor;
		vertexlist[5].pos = tmpPos;
	}
	{
		myVector4f tmpPos(CubeEdgeHalf, -CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(1.f, 1.f, 1.f);	//	w
		vertexlist[6].color = tmpColor;
		vertexlist[6].pos = tmpPos;
	}
	{
		myVector4f tmpPos(-CubeEdgeHalf, -CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.f, 0.f, 0.f);	//	bl
		vertexlist[7].color = tmpColor;
		vertexlist[7].pos = tmpPos;
	}

	glutMainLoop();
}

GLvoid RenderScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (isCullOn) { glEnable(GL_CULL_FACE); glFrontFace(GL_CCW); }
	else { glDisable(GL_CULL_FACE); }
	if (isDepthtestOn) { glEnable(GL_DEPTH_TEST); }
	else { glDisable(GL_DEPTH_TEST); }
	if (isSmoothOn) { glShadeModel(GL_SMOOTH); }
	else { glShadeModel(GL_FLAT); }

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

	drawSphereFront();
	drawSphereUp();
	drawSpring();

	/* 윗면 */
	glPushMatrix();
	glTranslatef(0.f, CubeEdgeHalf, -CubeEdgeHalf);
	glRotatef(upAngle, -1.f, 0.f, 0.f);
	glTranslatef(0.f, -CubeEdgeHalf, CubeEdgeHalf);
	drawFace(vertexlist[0], vertexlist[1], vertexlist[2], vertexlist[3]);
	glPopMatrix();

	drawFace(vertexlist[0], vertexlist[4], vertexlist[7], vertexlist[1]);
	drawFace(vertexlist[3], vertexlist[2], vertexlist[6], vertexlist[5]);

	/* 앞면 */
	glPushMatrix();
	glTranslatef(0.f, -CubeEdgeHalf, CubeEdgeHalf);
	glRotatef(frontAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, CubeEdgeHalf, -CubeEdgeHalf);
	drawFace(vertexlist[1], vertexlist[7], vertexlist[6], vertexlist[2]);
	glPopMatrix();

	drawFace(vertexlist[0], vertexlist[3], vertexlist[5], vertexlist[4]);
	drawFace(vertexlist[4], vertexlist[5], vertexlist[6], vertexlist[7]);

	drawAxis();

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
	if (isTopOpened) {
		/* 뚜껑 각 */
		if (upAngle < MAX_ANGLE) {
			upAngle += eTime * 100.f;
		}
		else if (upAngle > MAX_ANGLE) {
			upAngle = MAX_ANGLE;
		}
		/* 스프링 */
		if (upAngle > 90.f - FLT_EPSILON * 100.f && upAngle < 90.f + FLT_EPSILON * 100.f) {
			if (g_spring_height < MAX_SPRING_HEIGHT) {
				g_spring_height += eTime * 1000.f;
			}
			else if (g_spring_height > MAX_SPRING_HEIGHT) {
				g_spring_height = MAX_SPRING_HEIGHT;
			}
		}
	}
	else {
		/* 뚜껑 각 */
		if (g_spring_height > 0.f - FLT_EPSILON && g_spring_height < 0.f + FLT_EPSILON) {
			if (upAngle > MIN_ANGLE) {
				upAngle -= eTime * 100.f;
			}
			else if (upAngle < MIN_ANGLE) {
				upAngle = MIN_ANGLE;
			}
		}
		/* 스프링 */
		if (g_spring_height > 0.f) {
			g_spring_height -= eTime * 1000.f;
		}
		else if (g_spring_height < 0) {
			g_spring_height = 0.f;
		}
	}
	
	if (isFrontOpened) {
		/* 앞 뚜껑 */
		if (frontAngle < MAX_ANGLE) {
			frontAngle += eTime * 100.f;
		}
		else if (frontAngle > MAX_ANGLE) {
			frontAngle = MAX_ANGLE;
		}
		if (frontAngle > 90.f - FLT_EPSILON * 100.f && frontAngle < 90.f + FLT_EPSILON * 100.f) {
			if (g_frontSphereAngle < MAX_ANGLE) {
				g_frontSphereAngle += eTime * 100.f;
			}
			else if (g_frontSphereAngle > MAX_ANGLE) {
				g_frontSphereAngle = MAX_ANGLE;
			}
		}
		
	}
	else {
		/* 앞 뚜껑 */

		if (g_frontSphereAngle > 0.f - FLT_EPSILON * 100.f && g_frontSphereAngle < 0.f + FLT_EPSILON * 100.f) {
			if (frontAngle > MIN_ANGLE) {
				frontAngle -= eTime * 100.f;
			}
			else if (frontAngle < MIN_ANGLE) {
				frontAngle = MIN_ANGLE;
			}
		}
		if (g_frontSphereAngle > MIN_ANGLE) {
			g_frontSphereAngle -= eTime * 100.f;
		}
		else if (g_frontSphereAngle < MIN_ANGLE) {
			g_frontSphereAngle = MIN_ANGLE;
		}
		
	}
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


	case 'd':	/* 은면 제거 */
		if (isDepthtestOn) {
			isDepthtestOn = false;
		}
		else {
			isDepthtestOn = true;
		}
		break;

	case 'c':	/* 컬링 */
		if (isCullOn) {
			isCullOn = false;
		}
		else {
			isCullOn = true;
		}
		break;

	case 's':/* 쉐이딩 */
		if (isSmoothOn) {
			isSmoothOn = false;
		}
		else {
			isSmoothOn = true;
		}
		break;	

	case 'u':	/* 윗면 개폐 */
		if (isTopOpened) {
			isTopOpened = false;
		}
		else {
			isTopOpened = true;
		}
		break;

	case 'f':	/* 앞면 개폐 */
		if (isFrontOpened) {
			isFrontOpened = false;
		}
		else {
			isFrontOpened = true;
		}
		break; 

	case 'i':
	case 'I':
		{
			/* 카메라 초기화 */
			eye.x = 0.f;
			eye.y = 30.f;
			eye.z = 250.f;
		}
		break;

	case 'q':
	case VK_ESCAPE:
		glutLeaveMainLoop();
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

void drawSpring(void)
{
	glPointSize(1.f);
	glColor3f(1.f, 1.f, 1.f);
	for (float i = 0.f; i < 6.f * PI; i += 0.01) {	
		glBegin(GL_POINTS);
		glVertex3f(CubeEdgeHalf * cos(i), (g_spring_height / (6.f * PI)) * i, CubeEdgeHalf * sin(i));
		glEnd();
	}
}

void drawSphereFront(void)
{
	glPushMatrix();
	glColor3f(0.f, 1.f, 1.f);
	glTranslatef(0.f, 0.f, g_frontSphereAngle * convert_Deg2Radian * CubeEdgeHalf);
	glRotatef(g_frontSphereAngle, 1.f, 0.f, 0.f);
	glutWireSphere(CubeEdgeHalf, 20, 20);
	glPopMatrix();
}

void drawSphereUp(void)
{
	glPushMatrix();
	glColor3f(1.f, 1.f, 0.f);
	glTranslatef(0.f, g_spring_height, 0.f);
	glutSolidSphere(CubeEdgeHalf, 20, 20);
	glPopMatrix();
}

GLvoid drawFace(const myVertex& vertex1, const myVertex& vertex2, const myVertex& vertex3, const myVertex& vertex4)
{
	//glPointSize(5.f);
	//glBegin(GL_POINTS);
	glBegin(GL_QUADS);
	glColor3f(vertex1.color.r, vertex1.color.g, vertex1.color.b);
	glVertex3f(vertex1.pos.x, vertex1.pos.y, vertex1.pos.z);

	glColor3f(vertex2.color.r, vertex2.color.g, vertex2.color.b);
	glVertex3f(vertex2.pos.x, vertex2.pos.y, vertex2.pos.z);

	glColor3f(vertex3.color.r, vertex3.color.g, vertex3.color.b);
	glVertex3f(vertex3.pos.x, vertex3.pos.y, vertex3.pos.z);

	glColor3f(vertex4.color.r, vertex4.color.g, vertex4.color.b);
	glVertex3f(vertex4.pos.x, vertex4.pos.y, vertex4.pos.z);
	glEnd();
}
