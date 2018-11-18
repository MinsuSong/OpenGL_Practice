#include <gl/freeglut.h>
#include "CGMath.h"
#include "GlobalDefines.h"
#include <timeapi.h>
#include <float.h>

DWORD g_prevTime = 0;

constexpr int Max_Vertex{ 19 };

struct myVertex {
	bool isGenerated{ false };
};

myVertex generateList[19];

myVector4f vertexList[19];

GLvoid drawAxis();
void update(float eTime);

int g_currIndex{ 0 };
int g_selectIndex{ -1 };

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice28");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutMotionFunc(CB_mousemotionfunc);				//	MouseMotion
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutIdleFunc(CB_idle);							//	idle

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

	for (int i = 0; i < Max_Vertex - 1; i += 3) {
		glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 4, 4, vertexList[i].m);
		glEnable(GL_MAP1_VERTEX_3);

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 10; ++i) {
			glEvalCoord1f((GLfloat)i / 10.f);
		}
		glEnd();
		glDisable(GL_MAP1_VERTEX_3);
	}


	glPointSize(7.0);
	glColor3f(static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX, static_cast<double>(rand()) / RAND_MAX);
	glBegin(GL_POINTS);
	for (int i = 0; i < Max_Vertex; ++i) {
		if (generateList[i].isGenerated) { glVertex3f(vertexList[i].x, vertexList[i].y, vertexList[i].z); }
	}
	glEnd();

	glPushMatrix();

	update(eTime);

	
	drawAxis();

	glPopMatrix();


	glutSwapBuffers();	//	swap function of double buffering 
}

GLvoid ReShape(int w, int h)
{
	/* 화면 좌표계 설정 */
	glViewport(0, 0, w, h);
	/* 직교 투영 */
	glOrtho(-400.0, 400.0, -300.0, 300.0, -300.0, 300.0);

	glMatrixMode(GL_MODELVIEW);
}

void CB_idle()
{
	RenderScene();
}

void update(float eTime)
{
}

void CB_mousemovefunc(int button, int state, int x, int y)
{
	int mouseX = x - MAX_WINDOW_WIDTH;
	int mouseY = MY_WINDOW_HEIGHT - y - MAX_WINDOW_HEIGHT;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (g_currIndex < Max_Vertex) {
			vertexList[g_currIndex].x = mouseX;
			vertexList[g_currIndex].y = mouseY;
			generateList[g_currIndex].isGenerated = true;
			g_currIndex++;
			printf("%d\n", g_currIndex);
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		myVector4f tmpMousePos(mouseX, mouseY, 0);
		for (int i = 0; i < Max_Vertex; ++i) {
			if (10.f > getDistance(vertexList[i], tmpMousePos)) {
				g_selectIndex = i;
			}
		}
	}else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		g_selectIndex = -1;
	}
}

void CB_mousemotionfunc(int x, int y)
{
	if (g_selectIndex != -1) {
		int mouseX = x - MAX_WINDOW_WIDTH;
		int mouseY = MY_WINDOW_HEIGHT - y - MAX_WINDOW_HEIGHT;
		myVector4f tmpMousePos(mouseX, mouseY, 0);
		vertexList[g_selectIndex] = tmpMousePos;
	}
}

void CB_keydownfunc(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 'r':
	case 'R':
	{
		for (int i = 0; i < Max_Vertex; ++i) {
			generateList[i].isGenerated = false;
			vertexList[i] = zeroVector;
			g_currIndex = 0;
			g_selectIndex = -1;
		}
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
