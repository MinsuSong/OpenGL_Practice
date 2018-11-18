#include <gl/freeglut.h>
#include "CGMath.h"
#include "GlobalDefines.h"
#include <timeapi.h>
#include <float.h>

DWORD g_prevTime = 0;

#define MAX_BALLS	5
#define SPHERE_SIZE 5
#define MAX_RECTS	3

constexpr float CubeEdge{ 100.f };
constexpr float CubeEdgeHalf{ CubeEdge / 2.f };

myVector4f g_gravity(0.f, -196.f, 0.f);
myVector4f g_oldGravity(0.f, -19.6f, 0.f);

struct myVertex {
	myVector4f pos;
	myVector4f color;
};

struct mySphere {
	myVector4f pos;
	myVector4f vel;
	bool isGenerated{ false };
};

struct myRects {
	myVector4f pos;
	myVector4f vel;
	myVector4f accel;
	GLfloat size{ 0 };
};

void drawSpheres(void);
void drawRects(void);

GLvoid drawFace(const myVertex& vertex1, const myVertex& vertex2, const myVertex& vertex3, const myVertex& vertex4);
GLvoid drawAxis();
void drawGravityVector();
void update(float eTime);

GLvoid matrixProductX(myVector4f* v);
GLvoid matrixProductx(myVector4f* v);
GLvoid matrixProductY(myVector4f* v);
GLvoid matrixProducty(myVector4f* v);
GLvoid matrixProductZ(myVector4f* v);
GLvoid matrixProductz(myVector4f* v);

GLvoid matrixProcuctZrotate(myVector4f* gravity, float rad);

myVector4f eye{ 0.f,0.f,250.f };

myVertex vertexlist[8];
mySphere spherellist[MAX_BALLS];
myRects rectList[MAX_RECTS];
GLint g_sphereIndex{ 0 };

bool isCullOn{ true };
bool isDepthtestOn{ true };
bool isTopOpened{ false };
bool isFrontOpened{ false };
bool isSmoothOn{ false };

GLfloat g_WorldAngle{ 0.f };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice26");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutMotionFunc(CB_mousemotionfunc);				//	MouseMotion
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutIdleFunc(CB_idle);							//	idle


	{
		myVector4f tmpPos(-CubeEdgeHalf, CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.5f, 0.5f, 0.5f);	//	Saturation = hard gray
		vertexlist[0].color = tmpColor;
		vertexlist[0].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.8f, 0.8f, 0.8f);	//	Saturation = very hard gray
		vertexlist[1].color = tmpColor;
		vertexlist[1].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-CubeEdgeHalf, CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(0.3f, 0.3f, 0.3f);	//	Saturation = soft gray
		vertexlist[2].color = tmpColor;
		vertexlist[2].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(0.3f, 0.3f, 0.3f);	//	Saturation = soft gray
		vertexlist[3].color = tmpColor;
		vertexlist[3].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-CubeEdgeHalf, -CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.8f, 0.8f, 0.8f);	//	Saturation = very hard gray
		vertexlist[4].color = tmpColor;
		vertexlist[4].pos = tmpPos;
	}

	{
		myVector4f tmpPos(CubeEdgeHalf, -CubeEdgeHalf, CubeEdgeHalf);
		myVector4f tmpColor(0.5f, 0.5f, 0.5f);	//	Saturation = hard gray
		vertexlist[5].color = tmpColor;
		vertexlist[5].pos = tmpPos;
	}
	{
		myVector4f tmpPos(CubeEdgeHalf, -CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(0.3f, 0.3f, 0.3f);	//	Saturation = soft gray
		vertexlist[6].color = tmpColor;
		vertexlist[6].pos = tmpPos;
	}
	{
		myVector4f tmpPos(-CubeEdgeHalf, -CubeEdgeHalf, -CubeEdgeHalf);
		myVector4f tmpColor(0.3f, 0.3f, 0.3f);	//	Saturation = soft gray
		vertexlist[7].color = tmpColor;
		vertexlist[7].pos = tmpPos;
	}

	for (int i = 0; i < MAX_BALLS; ++i) {
		spherellist[i].pos = myVector4f(
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE,
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE,
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE
		);
		spherellist[i].vel = myVector4f(
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE,
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE,
			rand() % (50 - SPHERE_SIZE) - 50 + SPHERE_SIZE
		);
	}
	{
		rectList[0].size = 5.f;
		rectList[1].size = 10.f;
		rectList[2].size = 15.f;

		{
			myVector4f tmp(0.f, 0.f, -rectList[0].size * 2.f);
			rectList[1].pos = tmp;
		} 
		{
			myVector4f tmp(0.f, 0.f, -rectList[0].size * 2.f - rectList[1].size * 2.f);
			rectList[2].pos = tmp;
		}

	}

	glutMainLoop();
}

GLvoid RenderScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (isCullOn) { glEnable(GL_CULL_FACE); glFrontFace(GL_CW); }
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

	//drawGravityVector();

	glPushMatrix();
	glRotatef(g_WorldAngle, 0.f, 0.f, 1.f);

	update(eTime);

	drawRects();
	drawSpheres();

	/* 윗면 */
	drawFace(vertexlist[1], vertexlist[3], vertexlist[2], vertexlist[0]);

	drawFace(vertexlist[5], vertexlist[6], vertexlist[3], vertexlist[1]);
	drawFace(vertexlist[4], vertexlist[7], vertexlist[6], vertexlist[5]);
	drawFace(vertexlist[0], vertexlist[2], vertexlist[7], vertexlist[4]);
	drawFace(vertexlist[2], vertexlist[3], vertexlist[6], vertexlist[7]);

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
	for (int i = 0; i < MAX_BALLS; ++i) {
		if (spherellist[i].pos.x - SPHERE_SIZE < -CubeEdgeHalf) {
			spherellist[i].pos.x = -CubeEdgeHalf + SPHERE_SIZE;
			spherellist[i].vel.x = -spherellist[i].vel.x;
		}
		else if (spherellist[i].pos.x + SPHERE_SIZE > CubeEdgeHalf) {
			spherellist[i].pos.x = CubeEdgeHalf - SPHERE_SIZE;
			spherellist[i].vel.x = -spherellist[i].vel.x;
		}

		if (spherellist[i].pos.y - SPHERE_SIZE < -CubeEdgeHalf) {
			spherellist[i].pos.y = -CubeEdgeHalf + SPHERE_SIZE;
			spherellist[i].vel.y = -spherellist[i].vel.y;
		}
		else if (spherellist[i].pos.y + SPHERE_SIZE > CubeEdgeHalf) {
			spherellist[i].pos.y = CubeEdgeHalf - SPHERE_SIZE;
			spherellist[i].vel.y = -spherellist[i].vel.y;
		}

		if (spherellist[i].pos.z - SPHERE_SIZE < -CubeEdgeHalf) {
			spherellist[i].pos.z = -CubeEdgeHalf + SPHERE_SIZE;
			spherellist[i].vel.z = -spherellist[i].vel.z;
		}
		else if (spherellist[i].pos.z + SPHERE_SIZE > CubeEdgeHalf) {
			spherellist[i].pos.z = CubeEdgeHalf - SPHERE_SIZE;
			spherellist[i].vel.z = -spherellist[i].vel.z;
		}

		spherellist[i].pos = spherellist[i].pos + spherellist[i].vel * eTime;
	}

	for (int i = 0; i < MAX_RECTS; ++i) {
		rectList[i].vel = g_gravity;

		if (rectList[i].pos.x - rectList[i].size / 2.f < -CubeEdgeHalf) {
			rectList[i].pos.x = -CubeEdgeHalf + rectList[i].size / 2.f;
			//rectList[i].accel = -g_gravity;
			//rectList[i].vel = zeroVector;
		}
		else if (rectList[i].pos.x + rectList[i].size / 2.f > CubeEdgeHalf) {
			rectList[i].pos.x = CubeEdgeHalf - rectList[i].size / 2.f;
			//rectList[i].accel = -g_gravity;
			//rectList[i].vel = zeroVector;
		}

		if (rectList[i].pos.y - rectList[i].size / 2.f < -CubeEdgeHalf) {
			rectList[i].pos.y = -CubeEdgeHalf + rectList[i].size / 2.f;
			//rectList[i].accel = -g_gravity;
			//rectList[i].vel = zeroVector;
		}
		else if (rectList[i].pos.y + rectList[i].size / 2.f > CubeEdgeHalf) {
			rectList[i].pos.y = CubeEdgeHalf - rectList[i].size / 2.f;
			//rectList[i].accel = -g_gravity;
			//rectList[i].vel = zeroVector;
		}

		//rectList[i].vel = rectList[i].vel + rectList[i].accel * eTime;
		rectList[i].pos = rectList[i].pos + rectList[i].vel * eTime;
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
	case '+':
	{
		myVector4f tmp = normalize(eye);
		eye.x += tmp.x * 10;
		eye.y += tmp.y * 10;
		eye.z += tmp.z * 10;
	}
	break;
	case '-':
	{
		myVector4f tmp = normalize(eye);
		eye.x -= tmp.x * 10;
		eye.y -= tmp.y * 10;
		eye.z -= tmp.z * 10;
	}
	break;

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

	case 'b':
		if (g_sphereIndex < MAX_BALLS) {
			spherellist[g_sphereIndex].isGenerated = true;
			g_sphereIndex++;
		}
		break;

	case 'B':
		if (g_sphereIndex > -1) {
			spherellist[g_sphereIndex].isGenerated = false;
			g_sphereIndex--;
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


void drawSpheres(void)
{
	for (int i = 0; i < MAX_BALLS; ++i) {
		if (spherellist[i].isGenerated) {
			glPushMatrix();
			glColor3f(0.f, 0.f, 1.f);
			glTranslatef(spherellist[i].pos.x, spherellist[i].pos.y, spherellist[i].pos.z);
			glutSolidSphere(SPHERE_SIZE, 10, 10);
			glPopMatrix();
		}
	}
}

void drawRects(void)
{
	for (int i = 0; i < MAX_RECTS; ++i) {
		glPushMatrix();
		glTranslatef(rectList[i].pos.x, rectList[i].pos.y, rectList[i].pos.z);
		glScalef(rectList[i].size, rectList[i].size, rectList[i].size);
		glColor3f(1.f, 1.f, 1.f);
		glutWireCube(1);
		glColor3f(0.7f, 0.f, 0.f);
		glutSolidCube(1);
		glPopMatrix();

	}
}

void drawGravityVector()
{
	glLineWidth(10.f);
	glBegin(GL_LINES);	//	Y 축
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(g_gravity.x, g_gravity.y, g_gravity.z);
	glEnd();
}

int prev_x{ -1 };
float prevRad{ 0.f };

void CB_mousemotionfunc(int x, int y) {
	/* 상기 모범 답안 
	if (prev_x == -1) prev_x = x;
	int dx = prev_x - x;
	prev_x = x;
	*/



	int mouseX = x - MAX_WINDOW_WIDTH;
	//printf("%d\n", mouseX);

	float tmpRad = (360.f / MAX_WINDOW_WIDTH) * static_cast<float>(mouseX);
	if (prevRad > 0.f - FLT_EPSILON && prevRad < 0.f + FLT_EPSILON) { prevRad = tmpRad; }
	g_WorldAngle = tmpRad;
	matrixProcuctZrotate(&g_gravity, (tmpRad - prevRad) * convert_Deg2Radian);
	prevRad = tmpRad;
	RenderScene();
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

GLvoid matrixProcuctZrotate(myVector4f * gravity, float rad)
{
	myVector4f old = *gravity;
	printf("%f\n", rad);
	gravity->x = cos(-rad)* old.x - sin(-rad)* old.y;
	gravity->y = sin(-rad)* old.x + cos(-rad)* old.y;
}
