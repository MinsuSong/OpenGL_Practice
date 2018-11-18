#include <gl/freeglut.h>
#include "CGMath.h"
#include "GlobalDefines.h"
#include <timeapi.h>

DWORD g_prevTime = 0;

constexpr float MAX_ANGLE{ 90.f };
constexpr float MIN_ANGLE{ 0.f };
GLfloat frontAngle{ 0.f };

#define BODY_H	10.f 
#define LARM_H	50.f
#define UARM_H	45.f

#define NONE	0
#define RIGHT	1
#define TOP		-1
#define UP		1
#define LEFT	-1
#define BOTTOM	1
#define DOWN	-1
#define LARGE	1
#define SMALL	-1
#define FRONT	1
#define BACKWARD -1

constexpr float RobotSize{ 5.f };

bool isCullOn{ true };
bool isDepthtestOn{ true };
bool isFrontOpened{ false };

GLfloat global_RotateMatrix[16] = {
	1.f,0.f,0.f,0.f,
	0.f,1.f,0.f,0.f,
	0.f,0.f,1.f,0.f,
	0.f,0.f,0.f,1.f
};

struct myVertex {
	myVector4f pos;
	myVector4f color;
};

struct Object {
	myVector4f pos;
	myVector4f vel;
	GLfloat size{ 0.f };
	GLfloat scale{ 1.f };
	GLfloat Xangle{ 0.f };
	GLfloat Zangle{ 0.f };
};

struct DustCloud {
	GLfloat size{ 5.f };
	bool isAlive{ false };
	GLfloat oldAngle{ 0.f };
};

Object Robot;

myVertex vertexlist[8];

GLvoid drawFace(const myVertex& vertex1, const myVertex& vertex2, const myVertex& vertex3, const myVertex& vertex4);

void update(float eTime);
bool collide(const Object& lhs, const Object& rhs);
//bool collideRect(const Object& lhs, const Object& rhs);
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
GLvoid drawOrbit();
GLvoid drawAirplane();
GLvoid drawDustCloud(int i);
GLvoid drawRobot();

GLvoid matrixProductX(myVector4f* v);
GLvoid matrixProductx(myVector4f* v);
GLvoid matrixProductY(myVector4f* v);
GLvoid matrixProducty(myVector4f* v);
GLvoid matrixProductZ(myVector4f* v);
GLvoid matrixProductz(myVector4f* v);

myVector4f eye{ 0.f,300.f,550.f };

Object myCrane;
Object mySphere;
Object Tree;
Object PullupBar;
Object BenchPress;
Object RunningMachine;
Object Airplane;

GLfloat BODY_YAngle = 0.f;
GLfloat LARM_XAngle = 0.f;
GLfloat LARM_YAngle = 0.f;
GLfloat UARM_XAngle = 0.f;
GLfloat UARM_ZAngle = 0.f;
GLint g_TreeDir = LARGE;
GLfloat g_PullupAngle = 0.f;
GLfloat g_BenchBarHeight = 0.f;
GLfloat g_BenchBarDir = UP;
GLfloat g_RunningDir = FRONT;
GLfloat g_RunningAngle = 0.f;
GLfloat g_AirplaneAngle = 0.f;
GLfloat g_OrbitAngle = 0.f;
GLint g_AirplaneDir = 1;
GLfloat g_RobotAngle{ 0.f };

bool isRobotJumping{ false };

DustCloud g_dc[3];

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice27");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutIdleFunc(CB_idle);							//	idle
	//glEnable(GL_DEPTH_TEST);

	{
		myVector4f tmpVel(-100.f, 0.f, 0.f);
		myCrane.vel = tmpVel;
		myCrane.size = 25.f;
	}
	{
		mySphere.size = 20.f;
		myVector4f tmpVel(0.f, 0.f, 2 * PI * mySphere.size * 5.f / 360.f * 10.f);
		myVector4f tmpPos(50.f, 0.f, -100.f);
		mySphere.vel = tmpVel;
		mySphere.pos = tmpPos;
	}
	{
		Tree.size = 12.5f * sqrtf(2);
		myVector4f tmpPos(150.f - Tree.size * 2, 0.f, 150.f - Tree.size * 2);
		Tree.pos = tmpPos;
	}
	{
		PullupBar.size = 12.5f * sqrtf(2);
		myVector4f tmpPos(-150.f + PullupBar.size * 2, 0.f, -150.f + PullupBar.size * 2);
		PullupBar.pos = tmpPos;
	}
	{
		BenchPress.size = 12.5f * sqrtf(2);
		myVector4f tmpPos(150.f - BenchPress.size * 2, 0.f, -150.f + BenchPress.size * 2);
		BenchPress.pos = tmpPos;
	}
	{
		RunningMachine.size = 12.5f * sqrtf(2);
		myVector4f tmpPos(-150.f + RunningMachine.size * 2, 0.f, 150.f - RunningMachine.size * 2);
		RunningMachine.pos = tmpPos;
	}
	{
		myVector4f tmpPos(0.f, 100.f, 0.f);
		Airplane.pos = tmpPos;
	}

	{
		myVector4f tmpPos(-DEFAULT_GROUND_SIZE / 2.f, DEFAULT_GROUND_SIZE, DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.7f, 0.7f, 0.7f);
		vertexlist[0].color = tmpColor;
		vertexlist[0].pos = tmpPos;
	}

	{
		myVector4f tmpPos(DEFAULT_GROUND_SIZE / 2.f, DEFAULT_GROUND_SIZE, DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.6f, 0.6f, 0.6f);
		vertexlist[1].color = tmpColor;
		vertexlist[1].pos = tmpPos;
	}

	{
		myVector4f tmpPos(DEFAULT_GROUND_SIZE / 2.f, DEFAULT_GROUND_SIZE, -DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.7f, 0.7f, 0.7f);
		vertexlist[2].color = tmpColor;
		vertexlist[2].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-DEFAULT_GROUND_SIZE / 2.f, DEFAULT_GROUND_SIZE, -DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.6f, 0.6f, 0.6f);
		vertexlist[3].color = tmpColor;
		vertexlist[3].pos = tmpPos;
	}

	{
		myVector4f tmpPos(-DEFAULT_GROUND_SIZE / 2.f, 0.f, DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.7f, 0.7f, 0.7f);
		vertexlist[4].color = tmpColor;
		vertexlist[4].pos = tmpPos;
	}

	{
		myVector4f tmpPos(DEFAULT_GROUND_SIZE / 2.f, 0.f, DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.6f, 0.6f, 0.6f);
		vertexlist[5].color = tmpColor;
		vertexlist[5].pos = tmpPos;
	}
	{
		myVector4f tmpPos(DEFAULT_GROUND_SIZE / 2.f, 0.f, -DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.7f, 0.7f, 0.7f);
		vertexlist[6].color = tmpColor;
		vertexlist[6].pos = tmpPos;
	}
	{
		myVector4f tmpPos(-DEFAULT_GROUND_SIZE / 2.f, 0.f, -DEFAULT_GROUND_SIZE / 2.f);
		myVector4f tmpColor(0.6f, 0.6f, 0.6f);
		vertexlist[7].color = tmpColor;
		vertexlist[7].pos = tmpPos;
	}

	Robot.size = RobotSize;

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

	/* 윗면 */
	drawFace(vertexlist[0], vertexlist[3], vertexlist[2], vertexlist[1]);
	drawFace(vertexlist[1], vertexlist[2], vertexlist[6], vertexlist[5]);
	//drawFace(vertexlist[4], vertexlist[7], vertexlist[6], vertexlist[5]);
	drawFace(vertexlist[3], vertexlist[7], vertexlist[6], vertexlist[2]);
	drawFace(vertexlist[0], vertexlist[4], vertexlist[7], vertexlist[3]);
	/* 앞면 */
	glPushMatrix();
	glTranslatef(0.f, DEFAULT_GROUND_SIZE / 2.f, 0.f);
	glRotatef(frontAngle, -1.f, 0.f, 0.f);
	glTranslatef(0.f, -DEFAULT_GROUND_SIZE / 2.f, 0.f);
	drawFace(vertexlist[0], vertexlist[4], vertexlist[5], vertexlist[1]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(Robot.pos.x, Robot.pos.y, Robot.pos.z);
	glRotatef(g_RobotAngle, 0.f, 1.f, 0.f);
	drawRobot();
	glPopMatrix();

	drawGround();
	drawSector();
	drawAxis();
	drawAirplane();
	drawOrbit();
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
	/* 뚜껑 */
	if (isFrontOpened) {
		/* 앞 뚜껑 */
		if (frontAngle < MAX_ANGLE) {
			frontAngle += eTime * 100.f;
		}
		else if (frontAngle > MAX_ANGLE) {
			frontAngle = MAX_ANGLE;
		}
	}
	else {
		/* 앞 뚜껑 */

		if (frontAngle > MIN_ANGLE) {
			frontAngle -= eTime * 100.f;
		}
		else if (frontAngle < MIN_ANGLE) {
			frontAngle = MIN_ANGLE;
		}
	}

	/* 로봇 이동 */
	if (Robot.pos.y < 0) {
		Robot.vel.y = 0.f;
		Robot.pos.y = 0.f;
		isRobotJumping = false;
	}
	else if (Robot.pos.y > 20.f) {
		Robot.vel.y = -100.f;
	}

	if (Robot.pos.x + RobotSize > 150.f) {
		Robot.vel.x = 0.f;
		Robot.pos.x = 150.f - RobotSize;
	}
	else if (Robot.pos.x - RobotSize < -150.f) {
		Robot.vel.x = 0.f;
		Robot.pos.x = -150.f + RobotSize;
	}

	if (Robot.pos.z + RobotSize > 150.f) {
		Robot.vel.z = 0.f;
		Robot.pos.z = 150.f - RobotSize;
	}
	else if (Robot.pos.z - RobotSize < -150.f) {
		Robot.vel.z = 0.f;
		Robot.pos.z = -150.f + RobotSize;
	}


	/* 로봇 업데이트 */
	if (collide(Robot, RunningMachine)) {
		if (!isRobotJumping) {
			Robot.vel.y = 100.f;
			isRobotJumping = true;
		}
		myVector4f tmpReverse = Robot.pos - RunningMachine.pos;
		tmpReverse = normalize(tmpReverse) * 50.f;
		Robot.pos += tmpReverse;
	}
	else if (collide(Robot, PullupBar)) {
		if (!isRobotJumping) {
			Robot.vel.y = 100.f;
			isRobotJumping = true;
		}
		myVector4f tmpReverse = Robot.pos - PullupBar.pos;
		tmpReverse = normalize(tmpReverse) * 50.f;
		Robot.pos += tmpReverse;
	}
	else if (collide(Robot, BenchPress)) {
		if (!isRobotJumping) {
			Robot.vel.y = 100.f;
			isRobotJumping = true;
		}
		myVector4f tmpReverse = Robot.pos - BenchPress.pos;
		tmpReverse = normalize(tmpReverse) * 50.f;
		Robot.pos += tmpReverse;
	}
	else if (collide(Robot, Tree)) {
		if (!isRobotJumping) {
			Robot.vel.y = 100.f;
			isRobotJumping = true;
		}
		myVector4f tmpReverse = Robot.pos - Tree.pos;
		tmpReverse = normalize(tmpReverse) * 50.f;
		Robot.pos += tmpReverse;
	}
	
	Robot.pos = Robot.pos + Robot.vel * eTime;
	

	/* 다양한 애니메이션들 */
	if (Tree.scale > 1.2) { Tree.scale = 1.2; g_TreeDir = SMALL; }
	else if (Tree.scale < 0.8) { Tree.scale = 0.8; g_TreeDir = LARGE; }
	Tree.scale += g_TreeDir * eTime;

	g_PullupAngle += 100 * eTime;

	if (g_BenchBarHeight > 12.5f) { g_BenchBarHeight = 12.5f; g_BenchBarDir = DOWN; }
	else if (g_BenchBarHeight < 0.4f) { g_BenchBarHeight = 0.4f; g_BenchBarDir = UP; }
	g_BenchBarHeight += g_BenchBarDir * eTime * 10;

	if (g_RunningAngle > 30.f) { g_RunningAngle = 30.f, g_RunningDir = -1; }
	else if (g_RunningAngle < -30.f) { g_RunningAngle = -30.f, g_RunningDir = 1; }
	g_RunningAngle += g_RunningDir * eTime * 100;

	g_AirplaneAngle += 1000 * eTime;

	g_OrbitAngle += 100 * eTime * g_AirplaneDir;

	for (int i = 0; i < 3; ++i) {
		if (g_dc[i].size < 0) {
			g_dc[i].size = 7.f;
			g_dc[i].isAlive = false;
		}

		if (!g_dc[i].isAlive) {
			g_dc[i].oldAngle = g_OrbitAngle;
			g_dc[i].isAlive = true;
		}
		else {
			g_dc[i].size -= eTime * 10;
		}
	}
}

void CB_mousemovefunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	}
	RenderScene();
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

		/* 로봇 이동 */
	case 'a':
	{
		myVector4f tmpVel(-100.f, Robot.vel.y, 0.f);
		Robot.vel = tmpVel;
		g_RobotAngle = -90.f;
	}
		break;
	case 'w':
	{
		myVector4f tmpVel(0.f, Robot.vel.y, -100.f);
		Robot.vel = tmpVel; 
		g_RobotAngle = 180.f;
	}
		break;
	case 'd':
	{
		myVector4f tmpVel(100.f, Robot.vel.y, 0.f);
		Robot.vel = tmpVel;
		g_RobotAngle = 90.f;
	}
		break;
	case 's':
	{
		myVector4f tmpVel(0.f, Robot.vel.y, 100.f);
		Robot.vel = tmpVel;
		g_RobotAngle = 0.f;
	}
		break;
	case 'j':	/* 점프 */
	{
		isRobotJumping = true;
		Robot.vel.y = 100.f;
	}
		break;

	case 'D':	/* 은면 제거 */
	case 'g':
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
	case 'f':	/* 앞면 개폐 */
		if (isFrontOpened) {
			isFrontOpened = false;
		}
		else {
			isFrontOpened = true;
		}
		break;

	case 'q':
	case 'Q':
		glutLeaveMainLoop();

	case 'i':
	case 'I':
	{
		Robot.pos = zeroVector;
		Robot.vel = zeroVector;
		g_RobotAngle = 0.f;
		frontAngle = 0.f;
		isFrontOpened = false;
		/* 카메라 초기화 */
		eye.x = 0.f;
		eye.y = 300.f;
		eye.z = 550.f;
		/* 로봇 초기화 */
	}
		break;

	
	case 'v':
		if (g_AirplaneDir == 1) {
			g_AirplaneDir = -1;
		}
		else {
			g_AirplaneDir = 1;
		}
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
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, -1.f, 0.f);
	glScalef(DEFAULT_GROUND_SIZE, 1, DEFAULT_GROUND_SIZE);
	glutSolidCube(1);
	glPopMatrix();
}

GLvoid drawWorld()
{
	glLoadIdentity();
	//glPushMatrix();
	//glTranslatef(myCrane.pos.x, 0.f, myCrane.pos.z);
	//glTranslatef(0.f, BODY_H / 2.0f, 0.f);
	//glRotatef(BODY_YAngle, 0.f, 1.f, 0.f);
	//drawBody();
	//
	//glPushMatrix();
	//glTranslatef(0.f, BODY_H / 2.f, 0.f);
	//glRotatef(LARM_XAngle, 1.f, 0.f, 0.f);
	//glRotatef(LARM_YAngle, 0.f, 1.f, 0.f);
	//glTranslatef(0.f, LARM_H / 2.f, 0.f);
	//drawLowerArm();
	//
	//glPushMatrix();
	//glTranslatef(0.f, LARM_H / 2.f, 0.f);
	//glRotatef(UARM_XAngle, 1.f, 0.f, 0.f);
	//glRotatef(UARM_ZAngle, 0.f, 0.f, 1.f);
	//glTranslatef(0.f, UARM_H / 2.f, 0.f);
	//drawUpperArm();
	//
	//glPopMatrix();
	//glPopMatrix();
	//glPopMatrix();
	//
	//glPushMatrix();
	//glTranslatef(mySphere.pos.x, 0.f, mySphere.pos.z);
	//glTranslatef(0.f, mySphere.size, 0.f);
	//glMultMatrixf(global_RotateMatrix);
	//drawSphere();
	//glPopMatrix();

	glPushMatrix();
	glTranslatef(Tree.pos.x, 0.f, Tree.pos.z);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(PullupBar.pos.x, 0.f, PullupBar.pos.z);
	drawPullupBar();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(BenchPress.pos.x, 0.f, BenchPress.pos.z);
	drawBenchPress();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(RunningMachine.pos.x, 0.f, RunningMachine.pos.z);
	drawRunningMachine();
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


bool collide(const Object & lhs, const Object & rhs)
{	/* 사각형을 둘러싼 외접원을 통한 충돌 체크*/
	if (lhs.size + rhs.size > getDistance(lhs.pos, rhs.pos)) {
		return true;
	}
	return false;
}

GLvoid drawRunningMachine()
{
	glPushMatrix();
	glTranslatef(20.f, 5.f, 5.f);

	/* 휠 */
	glPushMatrix();
	glTranslatef(0.f, 0.f, -15.f);
	glScalef(5.f, 0.25f, 2.f);
	glRotatef(g_PullupAngle, -1.f, 0.f, 0.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glColor3f(0.f, 0.f, 0.f);
	glutSolidTorus(3.f, 5.f, 10, 10);
	glColor3f(1.f, 1.f, 1.f);
	glutWireTorus(3.f, 5.f, 10, 10);
	glPopMatrix();

	/* 왼 기둥 */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(-10.f, 12.5f, 0.f);
	glScalef(2.f, 25.f, 1.f);
	glutWireCube(1);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidCube(1);
	glPopMatrix();
	/* 오른 기둥 */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(10.f, 12.5f, 0.f);
	glScalef(2.f, 25.f, 1.f);
	glutWireCube(1);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidCube(1);
	glPopMatrix();
	/* 수평봉 */
	glPushMatrix();
	glTranslatef(0.f, 25.f, 0.f);
	glScalef(20.f, 2.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidCube(1);
	glPopMatrix();

	/* 다리 */
	glPushMatrix();
	glTranslatef(-1.f, 15.f, -10.f);
	glRotatef(g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -10.f, 0.f);
	glScalef(1.f, 10.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.f, 15.f, -10.f);
	glRotatef(-g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -10.f, 0.f);
	glScalef(1.f, 10.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.f, 15.f, -10.f);
	glScalef(5.f, 10.f, 2.f);
	glColor3f(1.f, 10.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.f, 15.f, 0.f);

	glPushMatrix();
	glTranslatef(-3.f, 5.f, -10.f);
	glRotatef(-g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -5.f, 0.f);
	glScalef(1.f, 7.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.f, 5.f, -10.f);
	glRotatef(g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -5.f, 0.f);
	glScalef(1.f, 7.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

GLvoid drawPullupBar()
{
	glPushMatrix();
	/*왼 기둥*/
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(-15.f, 25.f, 0.f);
	glScalef(2.f, 50.f, 5.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/*오른 기둥*/
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(15.f, 25.f, 0.f);
	glScalef(2.f, 50.f, 5.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/* 수평봉 */
	glPushMatrix();
	glTranslatef(0.f, 50.f, 0.f);
	glScalef(30.f, 2.f, 2.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/*사람*/
	glPushMatrix();

	glTranslatef(0.f, 50.f, 0.f);
	glRotatef(g_PullupAngle, 1.f, 0.f, 0.f);

	glPushMatrix();
	glTranslatef(5.f, 10.f, 0.f);
	glScalef(2.f, 15.f, 2.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.f, 10.f, 0.f);
	glScalef(2.f, 15.f, 2.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 15.f, 0.f);
	glScalef(5.f, 5.f, 5.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 20.f, 0.f);
	glScalef(10.f, 10.f, 2.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

GLvoid drawBenchPress()
{
	/*왼 기둥*/
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(-10.f, 20.f, 0.f);
	glScalef(2.f, 40.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/*오른 기둥*/
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(10.f, 20.f, 0.f);
	glScalef(2.f, 40.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/* 수평봉 */
	glPushMatrix();
	glTranslatef(0.f, 25.f, 0.f);
	glScalef(20.f, 2.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/* 벤치 */
	glPushMatrix();
	glTranslatef(0.f, 2.5, 12.5f);
	glScalef(20.f, 5.f, 25.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();
	/* 바 */
	glPushMatrix();
	glTranslatef(0.f, 25.f + g_BenchBarHeight, 5.f);
	glScalef(40.f, 2.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.3f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.f, 25.f + g_BenchBarHeight, 5.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidTorus(3.f, 5.f, 10, 10);
	glColor3f(1.f, 1.f, 1.f);
	glutWireTorus(3.f, 5.f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.f, 25.f + g_BenchBarHeight, 5.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidTorus(3.f, 5.f, 10, 10);
	glColor3f(1.f, 1.f, 1.f);
	glutWireTorus(3.f, 5.f, 10, 10);
	glPopMatrix();
}

GLvoid drawTree()
{
	/* brown body */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, 25.f, 0.f);
	glScalef(12.5f, 50.f, 12.5f);
	glutWireCube(1);
	glColor3f(0.7f, 0.3f, 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	/* green leaves */
	/* radius = 20 */
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, 50.f, 0.f);
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glScalef(Tree.scale, Tree.scale, Tree.scale);
	glutWireSphere(20.f, 10, 10);
	glColor3f(0.1f, 0.7f, 0.1f);
	glutSolidSphere(19.f, 20, 20);
	glPopMatrix();
}

GLvoid drawOrbit()
{
	glPushMatrix();
	glTranslatef(0.f, 100.f, 0.f);
	glRotatef(90.f, -1.f, 0.f, 0.f);
	glColor3f(1.f, 1.f, 1.f);
	glPointSize(1.f);
	for (float i = 0.f; i < 2 * PI; i += 0.01) {
		glBegin(GL_POINTS);
		glVertex3f(75.f * cos(i), 75.f * sin(i), 0.f);
		glEnd();
	}
	glPopMatrix();
}

GLvoid drawAirplane()
{
	glPushMatrix();
	glColor3f(1.f, 1.f, 1.f);
	glTranslatef(0.f, 100.f, 0.f);
	glRotatef(g_OrbitAngle, 0.f, 1.f, 0.f);
	glRotatef(90.f, -1.f, 0.f, 0.f);
	glTranslatef(75.f, 0.f, 0.f);
	if (g_AirplaneDir == -1) {
		glRotatef(180, 0.f, 0.f, 1.f);
	}

	/* wing */
	glPushMatrix();
	glScalef(25.5f, 5.5f, 0.25f);
	glutWireCube(1);
	glColor3f(1.f, 1.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();

	/* body */
	glPushMatrix();
	glScalef(3.5f, 12.5f, 3.5f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.f, 0.f, 0.7f);
	glutSolidCube(1);
	glPopMatrix();

	//drawAxis();

	/* tail*/
	glPushMatrix();
	glTranslatef(0.f, -6.f, 2.f);
	glScalef(1.f, 3.f, 4.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);
	glPopMatrix();


	/* propeller */
	{
		glPushMatrix();
		glRotatef(g_AirplaneAngle, 0.f, 1.f, 0.f);
		{
			glPushMatrix();
			glTranslatef(0.f, 8.f, 0.f);
			glScalef(10.5f, 2.5f, 1.5f);
			glColor3f(1.f, 1.f, 1.f);
			glutWireCube(1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.f, 8.f, 0.f);
			glRotatef(90.f, 0.f, 1.f, 0.f);
			glScalef(10.5f, 2.5f, 1.5f);
			glColor3f(1.f, 1.f, 1.f);
			glutWireCube(1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	for (int i = 0; i < 3; ++i) {
		if (g_dc[i].isAlive) {
			glPushMatrix();
			glColor3f(1.f, 1.f, 1.f);
			glTranslatef(0.f, 100.f, 0.f);
			glRotatef(g_dc[i].oldAngle - 30 * i * g_AirplaneDir, 0.f, 1.f, 0.f);
			glRotatef(90.f, -1.f, 0.f, 0.f);
			glTranslatef(75.f, 0.f, 0.f);
			if (g_AirplaneDir == -1) {
				glRotatef(180, 0.f, 0.f, 1.f);
			}

			drawDustCloud(i);

			glPopMatrix();
		}
	}
}

GLvoid drawDustCloud(int i)
{
	glPushMatrix();
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glColor3f(1.f, 1.f, 1.f);
	glScalef(g_dc[i].size, g_dc[i].size, g_dc[i].size);
	glutWireSphere(1.f, 10, 10);
	glColor3f(0.25f, 0.25f, 0.1f);
	glutSolidSphere(0.9f, 20, 20);
	glPopMatrix();
}

GLvoid drawRobot()
{
	/* 다리 */
	glPushMatrix();
	glTranslatef(-1.f, 15.f, 0.f);
	glRotatef(g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -10.f, 0.f);
	glScalef(1.f, 10.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.3f, 0.3f, 0.8f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.f, 15.f, 0.f);
	glRotatef(-g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -10.f, 0.f);
	glScalef(1.f, 10.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.3f, 0.3f, 0.8f);
	glutSolidCube(1);
	glPopMatrix();

	/* 몸통 */
	glPushMatrix();
	glTranslatef(0.f, 15.f, 0.f);
	glScalef(5.f, 10.f, 2.f);
	glColor3f(1.f, 10.f, 1.f);
	glutWireCube(1);
	glColor3f(0.3f, 0.5f, 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 15.f, 0.f);
	/* 두 팔 */
	glPushMatrix();
	glTranslatef(-3.f, 5.f, 0.f);
	glRotatef(-g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -5.f, 0.f);
	glScalef(1.f, 7.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.3f, 0.3f, 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.f, 5.f, 0.f);
	glRotatef(g_RunningAngle, 1.f, 0.f, 0.f);
	glTranslatef(0.f, -5.f, 0.f);
	glScalef(1.f, 7.f, 1.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(0.3f, 0.3f, 0.3f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 3.f, 1.f);
	glScalef(1.f, 1.f, 2.f);
	glColor3f(1.f, 1.f, 1.f);
	glutWireCube(1);
	glColor3f(1.f, 0.f, 0.f);
	glutSolidCube(1);

	glPopMatrix(); 
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