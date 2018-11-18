#include <gl/freeglut.h>
#include "myHeader.h"

#define LEFT			1
#define RIGHT			2
#define UP				3
#define DOWN			4
#define CLOCKWISE		5
#define ANTI_CLOCKWISE	6

GLvoid drawAxis();
GLvoid drawGround();
GLvoid convertScreen();
GLvoid drawShape();


GLvoid matrixProductX(Vector* v);
GLvoid matrixProductx(Vector* v);
GLvoid matrixProductY(Vector* v);
GLvoid matrixProducty(Vector* v);
GLvoid matrixProductZ(Vector* v);
GLvoid matrixProductz(Vector* v);

GLint Xrotate{ 0 };
GLint Yrotate{ 0 };
GLint Zrotate{ 0 };
GLint xrotate{ 0 };
GLint yrotate{ 0 };
GLint zrotate{ 0 };

GLvoid rotateSphere();


Vector eye{ 0.f,150.f,250.f };


struct SHAPEs {
	GLfloat x{ 0 };
	GLfloat y{ 0 };
	GLfloat z{ 0 };
	GLfloat size{ 0 };
};

Vector Xaxis{ 1.f,0.f,0.f };
Vector Yaxis{ 0.f,1.f,0.f };
Vector Zaxis{ 0.f,0.f,1.f };

SHAPEs sphere;

void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//	double buffering display mode initiation
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(static_cast<int>(MY_WINDOW_WIDTH), static_cast<int>(MY_WINDOW_HEIGHT));
	glutCreateWindow("Practice19ERROR");
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
			sphere.x = 0.f;
			sphere.y = 0.f;
			sphere.z = 0.f;
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
			break;


		case 'a':
			printf("hello\n");
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


		case 'l':
			if (sphere.x * PI * 20.f / 36.f < 150.f) {
				sphere.x++;
			}
			xrotate++;
			break;
		case 'L':
			if (sphere.x * PI * 20.f / 36.f > -150.f) {
				sphere.x--;
			}
			Xrotate++;
			break;
		case 'm':
			yrotate++;
			break;
		case 'M':
			Yrotate++;
			break;
		case 'n':
			if (sphere.z * PI * 20.f / 36.f < 150.f) {
				sphere.z++;
			}
			zrotate++;
			break;
		case 'N':
			if (sphere.z * PI * 20.f / 36.f > -150.f) {
				sphere.z--;
			}
			Zrotate++;
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

GLvoid drawShape()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(sphere.x * PI * 20.f / 36.f, 0, sphere.z * PI * 20.f / 36.f);
	glTranslatef(0.f, 20.f, 0.f);
	
	rotateSphere();

	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glColor3f(0.3f, 0.5f, 1.f);
	glutWireSphere(20.0, 20, 20);
	glColor3f(0.9f, 0.9f, 0.9f);
	glutSolidSphere(19.0, 20, 20);

	glPushMatrix();
	glScalef(0.8f, 0.8f, 0.8f);
	drawAxis();
	glPopMatrix();
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

GLvoid rotateSphere()
{
	for (int i = 0; i < xrotate; ++i) {
		glRotatef(-5.f, 0.f, 0.f, 1.f);
	}
	for (int i = 0; i < Xrotate; ++i) {
		glRotatef(5.f, 0.f, 0.f, 1.f);
	}
	for (int i = 0; i < yrotate; ++i) {
		glRotatef(5.f, 0.f, 1.f, 0.f);
	}
	for (int i = 0; i < Yrotate; ++i) {
		glRotatef(-5.f, 0.f, 1.f, 0.f);
	}
	for (int i = 0; i < zrotate; ++i) {
		glRotatef(5.f, 1.f, 0.f, 0.f);
	}
	for (int i = 0; i < Zrotate; ++i) {
		glRotatef(-5.f, 1.f, 0.f, 0.f);
	}
}
