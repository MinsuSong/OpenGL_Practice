#include <gl/freeglut.h>
#include "GlobalDefines.h"
#include "CGMath.h"
#include "Object.h"
#include "ProjectDefines.h"
#include <timeapi.h>

DWORD g_prevTime = 0;
bool isMouseClicked = false;
bool isMouseDragging = false;
myVector4f mousePoints[2];
int g_starIndex = 0;

GLvoid drawAxis();
GLvoid drawGrid();
GLvoid drawMouseLine();

Object test;
Object menubarTriangles[MAX_MENUTRIANGLES];	/* global objects */
Object Rectangles[MAX_RECTANGLES];			/* global objects */
Object leftSliceTriangles[MAX_SLICETRIANGLES];
Object rightSliceTriangles[MAX_SLICETRIANGLES];
Object Stars[MAX_STARS];
Object setLeftTriangles[MAX_CELL];
Object setRightTriangles[MAX_CELL];

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("숙제 1");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReShape);

	//	Call Back Fuctions
	glutMouseFunc(CB_mousemovefunc);				//	Mouse
	glutKeyboardFunc(CB_keydownfunc);				//	Keyboard
	glutMotionFunc(CB_mousemotionfunc);				//	MouseMotion
	glutIdleFunc(CB_idle);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	Rectangles[0].setPos(myVector4f(0.f, MIN_WINDOW_HEIGHT - rectEdge, 0.f));
	Rectangles[1].setPos(myVector4f(0.f, 2.f * MIN_WINDOW_HEIGHT - rectEdge, 0.f));
	for (int i = 0; i < MAX_RECTANGLES; ++i) {
		Rectangles[i].setVel(myVector4f(0.f, 100.f, 0.f));
		Rectangles[i].setColor(myVector4f(0.7f, 0.3f, 0.3f));
		Rectangles[i].setShape(rand() % 2 + 1);
		Rectangles[i].setLifeSign(true);
	}
	for (int i = 0; i < MAX_MENUTRIANGLES; ++i) {
		menubarTriangles[i].setPos(myVector4f(MIN_WINDOW_WIDTH - 2 * rectEdge * static_cast<float>(i), MAX_WINDOW_HEIGHT - rectEdge, 0.f));
		menubarTriangles[i].setVel(myVector4f(100.f, 0.f, 0.f));
		menubarTriangles[i].setColor(myVector4f(0.f, 1.f, 0.25f));
		menubarTriangles[i].setShape(TRIANGLE);
		menubarTriangles[i].setAngle(static_cast<GLfloat>(rand() % 360));
		menubarTriangles[i].setLifeSign(true);
	}

	for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
		leftSliceTriangles[i].setLifeSign(false);
		rightSliceTriangles[i].setLifeSign(false);
	}

	for (int i = 0; i < MAX_STARS; ++i) {
		Stars[i].setLifeSign(false);
		Stars[i].setShape(STAR);
	}

	for (int i = 0; i < MAX_CELL; ++i) {
		setLeftTriangles[i].setShape(TRIANGLE);
		setLeftTriangles[i].setLifeSign(true);
		setLeftTriangles[i].setColor(myVector4f(0.f, 1.f, 0.25f));
		setRightTriangles[i].setShape(TRIANGLE);
		setRightTriangles[i].setLifeSign(true);
		if (i % 2 == 0) {
			setLeftTriangles[i].setAngle(180);
			setRightTriangles[i].setAngle(-90);
		}
		else {
			setRightTriangles[i].setAngle(90);
		}
		setLeftTriangles[i].setPos(
			myVector4f(MIN_WINDOW_WIDTH - rectEdgeHalf - rectEdge * i, MIN_WINDOW_HEIGHT + rectEdgeHalf + rectEdge * (i % 11), 0.f)
		);
	}
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
	float eTime = static_cast<float>(elapsedTime) / 1000.f; /* DWORD 자료형을 float으로 */
	//printf("%f\n", eTime);	//*********************** TEST

	glPushMatrix();

	drawMouseLine();

	for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {	/* 잘린 왼쪽 삼각형 drawing */
		glPushMatrix();
		myVector4f tmpPos;
		leftSliceTriangles[i].update(eTime);
		leftSliceTriangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(leftSliceTriangles[i].getAngle(), 0.f, 0.f, 1.f);
		leftSliceTriangles[i].drawShape();
		glPopMatrix();
	}
	for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {	/* 잘린 오른쪽 삼각형 drawing */
		glPushMatrix();
		myVector4f tmpPos;
		rightSliceTriangles[i].update(eTime);
		rightSliceTriangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(rightSliceTriangles[i].getAngle(), 0.f, 0.f, 1.f);
		rightSliceTriangles[i].drawShape();
		glPopMatrix();
	}

	/* 별 생성 */
	for (int i = 0; i < MAX_STARS; ++i) {
		glPushMatrix();
		myVector4f tmpPos;
		GLfloat size = Stars[i].getSize();
		Stars[i].update(eTime);
		Stars[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(Stars[i].getAngle(), 1.f, 1.f, 1.f);
		glScalef(size, size, size);
		Stars[i].drawShape();
		glPopMatrix();
	}

	/* 메뉴바 삼각형 drawing */
	for (int i = 0; i < MAX_MENUTRIANGLES; ++i) {
		glPushMatrix();
		myVector4f tmpPos;
		menubarTriangles[i].update(eTime);
		menubarTriangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(menubarTriangles[i].getAngle(), 0.f, 0.f, 1.f);
		menubarTriangles[i].drawShape();
		glPopMatrix();
	}
	/* 자를 사각형 drawing */
	for (int i = 0; i < MAX_RECTANGLES; ++i) {
		glPushMatrix();
		myVector4f tmpPos;
		Rectangles[i].update(eTime);
		Rectangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		Rectangles[i].drawShape();
		glPopMatrix();
	}

	/* 프레임 삼각형 drawing */
	for (int i = 0; i < MAX_CELL; ++i) {
		glPushMatrix();
		myVector4f tmpPos;
		setRightTriangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(setRightTriangles[i].getAngle(), 0.f, 0.f, 1.f);
		setRightTriangles[i].drawShape();
		glPopMatrix();

		glPushMatrix();
		setLeftTriangles[i].getPos(&tmpPos);
		glTranslatef(tmpPos.x, tmpPos.y, tmpPos.z);
		glRotatef(setLeftTriangles[i].getAngle(), 0.f, 0.f, 1.f);
		setLeftTriangles[i].drawShape();
		glPopMatrix();
	}

	drawGrid();
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

void CB_mousemovefunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//printf("hello\n");
		mousePoints[0].x = x - MAX_WINDOW_WIDTH;
		mousePoints[0].y = MY_WINDOW_HEIGHT - y - MAX_WINDOW_HEIGHT;
		isMouseClicked = true;

		myVector4f mousePoint(mousePoints[0].x, mousePoints[0].y, 0.f);

		for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
			myVector4f tmpPos;
			myVector4f zeroVector(0.f, 0.f, 0.f);
			if (leftSliceTriangles[i].getLifeSign()) {
				leftSliceTriangles[i].getPos(&tmpPos);
				if (((sqrtf(2.f) * rectEdge) / 2.f) > getDistance(tmpPos, mousePoint)) {
					//printf("왼쪽충돌\n"); //*********************** TEST
					leftSliceTriangles[i].setMouseClicked(true);
					leftSliceTriangles[i].setVel(zeroVector);
					leftSliceTriangles[i].setAccel(zeroVector);
					leftSliceTriangles[i].setState(STATIC);
					leftSliceTriangles[i].setPos(mousePoint);
				}
			}
			if (rightSliceTriangles[i].getLifeSign()) {
				rightSliceTriangles[i].getPos(&tmpPos);
				if (((sqrtf(2.f) * rectEdge) / 2.f) > getDistance(tmpPos, mousePoint)) {
					//printf("오른충돌\n"); //*********************** TEST
					rightSliceTriangles[i].setMouseClicked(true);
					rightSliceTriangles[i].setVel(zeroVector);
					rightSliceTriangles[i].setAccel(zeroVector);
					rightSliceTriangles[i].setState(STATIC);
					rightSliceTriangles[i].setPos(mousePoint);
				}
			}
		}

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		for (int i = 0; i < MAX_RECTANGLES; ++i) {
			/* 사각형 썰기 충돌체크 및 그에 따른 절단삼각형 생성 */
			if (Rectangles[i].getLifeSign()) {
				myVector4f tmpPos;
				Rectangles[i].getPos(&tmpPos);
				if (Rectangles[i].getShape() == DIAMOND) {
					if (tmpPos.x - rectEdge * sqrtf(2.f) / 2.f > mousePoints[0].x
						&&
						tmpPos.x + rectEdge * sqrtf(2.f) / 2.f < mousePoints[1].x
						&&
						tmpPos.y + rectEdge > mousePoints[0].y
						&&
						tmpPos.y + rectEdge > mousePoints[1].y
						&&
						tmpPos.y - rectEdge < mousePoints[0].y
						&&
						tmpPos.y - rectEdge < mousePoints[1].y
						)
					{	/* 좌에서 우 */
						Rectangles[i].setLifeSign(false);
						//printf("slice L 2 R\n");	//*********************** TEST
						{
							leftSliceTriangles[i].setPos(tmpPos);
							leftSliceTriangles[i].setAngle(-135.f);
							leftSliceTriangles[i].setLifeSign(true);
							leftSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(-(rand() % 300 + 100.f), rand() % 300 + 100.f, 0.f);
							leftSliceTriangles[i].setVel(tmpVel);
							leftSliceTriangles[i].setState(DYNAMIC);
							leftSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
						{
							rightSliceTriangles[i].setPos(tmpPos);
							rightSliceTriangles[i].setAngle(45.f);
							rightSliceTriangles[i].setLifeSign(true);
							rightSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(rand() % 300 + 100.f, rand() % 300 + 100.f, 0.f);
							rightSliceTriangles[i].setVel(tmpVel);
							rightSliceTriangles[i].setState(DYNAMIC);
							rightSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
					}
					else if (tmpPos.y - rectEdge * sqrtf(2.f) / 2.f > mousePoints[1].y
						&&
						tmpPos.y + rectEdge * sqrtf(2.f) / 2.f < mousePoints[0].y
						&&
						tmpPos.x + rectEdge > mousePoints[0].x
						&&
						tmpPos.x + rectEdge > mousePoints[1].x
						&&
						tmpPos.x - rectEdge < mousePoints[0].x
						&&
						tmpPos.x - rectEdge < mousePoints[1].x
						)
					{	/* 상에서 하 */
						Rectangles[i].setLifeSign(false);
						//printf("slice T 2 D\n");	//*********************** TEST
						{
							leftSliceTriangles[i].setPos(tmpPos);
							leftSliceTriangles[i].setAngle(-45.f);
							leftSliceTriangles[i].setLifeSign(true);
							leftSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(-(rand() % 300 + 100.f), rand() % 300 + 100.f, 0.f);
							leftSliceTriangles[i].setVel(tmpVel);
							leftSliceTriangles[i].setState(DYNAMIC);
							leftSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
						{
							rightSliceTriangles[i].setPos(tmpPos);
							rightSliceTriangles[i].setAngle(135.f);
							rightSliceTriangles[i].setLifeSign(true);
							rightSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(rand() % 300 + 100.f, rand() % 300 + 100.f, 0.f);
							rightSliceTriangles[i].setVel(tmpVel);
							rightSliceTriangles[i].setState(DYNAMIC);
							rightSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
					}
				}
				else if (Rectangles[i].getShape() == RECTANGLE) {
					if (mousePoints[0].x < tmpPos.x - rectEdge
						&&
						mousePoints[0].y > tmpPos.y + rectEdge
						&&
						mousePoints[1].x > tmpPos.x + rectEdge
						&&
						mousePoints[1].y < tmpPos.y - rectEdge
						)
					{
						Rectangles[i].setLifeSign(false);
						//printf("slice LT 2 RB\n");	//*********************** TEST
						{
							leftSliceTriangles[i].setPos(tmpPos);
							leftSliceTriangles[i].setAngle(0.f);
							leftSliceTriangles[i].setLifeSign(true);
							leftSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(-(rand() % 300 + 100.f), rand() % 300 + 100.f, 0.f);
							leftSliceTriangles[i].setVel(tmpVel);
							leftSliceTriangles[i].setState(DYNAMIC);
							leftSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
						{
							rightSliceTriangles[i].setPos(tmpPos);
							rightSliceTriangles[i].setAngle(180.f);
							rightSliceTriangles[i].setLifeSign(true);
							rightSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(rand() % 300 + 100.f, rand() % 300 + 100.f, 0.f);
							rightSliceTriangles[i].setVel(tmpVel);
							rightSliceTriangles[i].setState(DYNAMIC);
							rightSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
					}
					else if (mousePoints[0].x > tmpPos.x + rectEdge
						&&
						mousePoints[0].y > tmpPos.y + rectEdge
						&&
						mousePoints[1].x < tmpPos.x - rectEdge
						&&
						mousePoints[1].y < tmpPos.y - rectEdge
						)
					{
						Rectangles[i].setLifeSign(false);
						//printf("slice RT 2 LB\n");	//*********************** TEST
						{
							leftSliceTriangles[i].setPos(tmpPos);
							leftSliceTriangles[i].setAngle(-90.f);
							leftSliceTriangles[i].setLifeSign(true);
							leftSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(-(rand() % 300 + 100.f), rand() % 300 + 100.f, 0.f);
							leftSliceTriangles[i].setVel(tmpVel);
							leftSliceTriangles[i].setState(DYNAMIC);
							leftSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
						{
							rightSliceTriangles[i].setPos(tmpPos);
							rightSliceTriangles[i].setAngle(90.f);
							rightSliceTriangles[i].setLifeSign(true);
							rightSliceTriangles[i].setShape(TRIANGLE);
							myVector4f tmpVel(rand() % 300 + 100.f, rand() % 300 + 100.f, 0.f);
							rightSliceTriangles[i].setVel(tmpVel);
							rightSliceTriangles[i].setState(DYNAMIC);
							rightSliceTriangles[i].setColor(myVector4f(1.f, 0.f, 1.f));
						}
					}
				}
			}
		}

		/* if LBUTTON UP -> let triangles go */
		for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
			leftSliceTriangles[i].setMouseClicked(false);
			leftSliceTriangles[i].setState(DYNAMIC);
			rightSliceTriangles[i].setMouseClicked(false);
			rightSliceTriangles[i].setState(DYNAMIC);
		}
		for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
			myVector4f tmpSlicePos;
			myVector4f zeroVector(0.f, 0.f, 0.f);
			leftSliceTriangles[i].getPos(&tmpSlicePos);
			for (int j = 0; j < MAX_MENUTRIANGLES; ++j) {
				myVector4f tmpMenuPos;
				if (menubarTriangles[j].getLifeSign()) {
					menubarTriangles[j].getPos(&tmpMenuPos);
					if (rectEdgeHalf > getDistance(tmpSlicePos, tmpMenuPos)) {
						Stars[g_starIndex].setPos(tmpMenuPos);
						Stars[g_starIndex].setLifeSign(true);
						myVector4f tmpDest(rand() % 770 - 400 + rectEdge, rand() % 360 - 150 + rectEdge, 0.f);
						Stars[g_starIndex].setDestination(tmpDest);
						myVector4f velocity = tmpDest - tmpMenuPos;
						//velocity = velocity / getDistance(tmpDest, tmpMenuPos);
						Stars[g_starIndex].setVel(velocity);
						Stars[g_starIndex].setSize((double)rand() / RAND_MAX + 1.f);
						Stars[g_starIndex].setAngle(rand() % 360);
						menubarTriangles[j].setLifeSign(false);
						leftSliceTriangles[i].setLifeSign(false);
						leftSliceTriangles[i].setPos(zeroVector);
						g_starIndex++;
					}
				}
			}
			rightSliceTriangles[i].getPos(&tmpSlicePos);
			for (int j = 0; j < MAX_MENUTRIANGLES; ++j) {
				myVector4f tmpMenuPos;
				if (menubarTriangles[j].getLifeSign()) {
					menubarTriangles[j].getPos(&tmpMenuPos);
					if (rectEdgeHalf > getDistance(tmpSlicePos, tmpMenuPos)) {
						Stars[g_starIndex].setPos(tmpMenuPos);
						Stars[g_starIndex].setLifeSign(true);
						myVector4f tmpDest(rand() % 770 - 400 + rectEdge, rand() % 360 - 150 + rectEdge, 0.f);
						Stars[g_starIndex].setDestination(tmpDest);
						myVector4f velocity = tmpDest - tmpMenuPos;
						//velocity = velocity / getDistance(tmpDest, tmpMenuPos);
						Stars[g_starIndex].setVel(velocity);
						Stars[g_starIndex].setSize((double)rand() / RAND_MAX + 1.f);
						Stars[g_starIndex].setAngle(rand() % 360);
						menubarTriangles[j].setLifeSign(false);
						rightSliceTriangles[i].setLifeSign(false);
						rightSliceTriangles[i].setPos(zeroVector);
						g_starIndex++;
					}
				}
			}
		}
		isMouseClicked = false;
		isMouseDragging = false;
	}
	RenderScene();
}

void CB_mousemotionfunc(int x, int y)
{
	mousePoints[1].x = x - MAX_WINDOW_WIDTH;
	mousePoints[1].y = MY_WINDOW_HEIGHT - y - MAX_WINDOW_HEIGHT;
	myVector4f mousePoint(mousePoints[1].x, mousePoints[1].y, 0.f);

	myVector4f zeroVector(0.f, 0.f, 0.f);
	for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
		if (leftSliceTriangles[i].getMouseClicked()) {
			leftSliceTriangles[i].setPos(mousePoint);
			leftSliceTriangles[i].setVel(zeroVector);
			leftSliceTriangles[i].setAccel(zeroVector);
		}
	}
	for (int i = 0; i < MAX_SLICETRIANGLES; ++i) {
		if (rightSliceTriangles[i].getMouseClicked()) {
			rightSliceTriangles[i].setPos(mousePoint);
			rightSliceTriangles[i].setVel(zeroVector);
			rightSliceTriangles[i].setAccel(zeroVector);
		}
	}

	if (isMouseClicked) {
		isMouseDragging = true;
	}
	RenderScene();
}

void CB_keydownfunc(unsigned char key, int x, int y)
{
	switch (key)
	{
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
	ReShape(800, 600);
	glutPostRedisplay();
}

void CB_idle()
{
	RenderScene();
}

GLvoid drawAxis(void)
{
	/* 실습 Test용 좌표축 */
	glLineWidth(3.f);
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
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0, 0.f, 0.f);
	glVertex3f(sqrtf(2.f) * rectEdgeHalf / 2.f, 0.f, 0.f);
	glEnd();
	glLineWidth(1.f);
}

GLvoid drawGrid()
{
	glLineWidth(1.f);
	glColor3f(0.9f, 0.9f, 0.9f);

	/* 그릴 도형의 크기에 꼭 맞게 Grid Frame을 구성하기 위해서 */

	/* draw Horizontal Lines */
	for (float i = 0.f; i < MAX_GRID_ROW; ++i) {
		glBegin(GL_LINES);
		glVertex3f(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT + i * rectEdge, 0.f);
		glVertex3f(MIN_WINDOW_WIDTH + rectEdge * (MAX_GRID_COLUMN - 1), MIN_WINDOW_HEIGHT + i * rectEdge, 0.f);
		glEnd();
	}
	for (float i = 0.f; i < MAX_GRID_ROW; ++i) {
		glBegin(GL_LINES);
		glVertex3f(MAX_WINDOW_WIDTH - rectEdge * (MAX_GRID_COLUMN - 1), MIN_WINDOW_HEIGHT + i * rectEdge, 0.f);
		glVertex3f(MAX_WINDOW_WIDTH, MIN_WINDOW_HEIGHT + i * rectEdge, 0.f);
		glEnd();
	}

	/* draw Vertical Lines */
	for (float i = 0.f; i < MAX_GRID_COLUMN; ++i) {
		glBegin(GL_LINES);
		glVertex3f(MIN_WINDOW_WIDTH + i * rectEdge, MIN_WINDOW_HEIGHT + (MAX_GRID_ROW - 1) * rectEdge, 0.f);
		glVertex3f(MIN_WINDOW_WIDTH + i * rectEdge, MIN_WINDOW_HEIGHT, 0.f);
		glEnd();
	}
	for (float i = 0.f; i < MAX_GRID_COLUMN; ++i) {
		glBegin(GL_LINES);
		glVertex3f(MAX_WINDOW_WIDTH - i * rectEdge, MIN_WINDOW_HEIGHT + (MAX_GRID_ROW - 1) * rectEdge, 0.f);
		glVertex3f(MAX_WINDOW_WIDTH - i * rectEdge, MIN_WINDOW_HEIGHT, 0.f);
		glEnd();
	}

	/* draw upper boundary */
	/* 루트 2가 1.4니까, 삼각형 회전했을 때 최대 도형높이보다 약간 더 크게 이격시키기 위함 */
	glBegin(GL_LINES);
	glVertex3f(MIN_WINDOW_WIDTH, MAX_WINDOW_HEIGHT - 2.f * rectEdge, 0.f);
	glVertex3f(MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT - 2.f * rectEdge, 0.f);
	glEnd();
}

GLvoid drawMouseLine()
{
	if (isMouseClicked && isMouseDragging) {
		glLineWidth(1.f);
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_LINES);
		glVertex3f(mousePoints[0].x, mousePoints[0].y, 0.f);
		glVertex3f(mousePoints[1].x, mousePoints[1].y, 0.f);
		glEnd();
	}
}
