#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/glut.h>
#include <stdio.h> 
#include <math.h>  
#include <iostream>
#include <time.h>

GLvoid drawScene(GLvoid);
GLvoid reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);

void TIME();

GLvoid DrawLine()
{
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 800, 0);
	glVertex3f(0, -800, 0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(800, 0, 0);
	glVertex3f(-800, 0, 0);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 800);
	glVertex3f(0, 0, -800);
	glEnd();
}


struct Cube
{
	float scaleX = 1;
	float scaleY = 1;
	float scaleZ = 1;

	int posX = 0;
	int posY = 0;
	int posZ = 0;

	float transX = 0;
	float transY = 0;
	float transZ = 0;

	int flag = 0;
	int time = 0;
};

Cube cube[6][7];



void main(int argc, char** argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("sad");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(10, TimerFunction, 1);
	glutReshapeFunc(reshape);
	//Init
	for (int i = 0; i < 6; ++i)
	{
		cube[i][0].scaleX = 0.25;
		cube[i][0].scaleY = 2;
		cube[i][0].scaleZ = 1;
		cube[i][0].posX = -2972;
		cube[i][0].posY = 60;
		cube[i][0].posZ = 0;

		cube[i][1].scaleX = 1;
		cube[i][1].scaleY = 0.4;
		cube[i][1].scaleZ = 1;
		cube[i][1].posX = -680;
		cube[i][1].posY = 0;
		cube[i][1].posZ = 0;

		cube[i][2].scaleX = 0.25;
		cube[i][2].scaleY = 2;
		cube[i][2].scaleZ = 1;
		cube[i][2].posX = -2467;
		cube[i][2].posY = 60;
		cube[i][2].posZ = 0;

		cube[i][3].scaleX = 1;
		cube[i][3].scaleY = 0.4;
		cube[i][3].scaleZ = 1;
		cube[i][3].posX = -680;
		cube[i][3].posY = 600;
		cube[i][3].posZ = 0;

		cube[i][4].scaleX = 0.25;
		cube[i][4].scaleY = 2;
		cube[i][4].scaleZ = 1;
		cube[i][4].posX = -2972;
		cube[i][4].posY = -60;
		cube[i][4].posZ = 0;

		cube[i][5].scaleX = 1;
		cube[i][5].scaleY = 0.4;
		cube[i][5].scaleZ = 1;
		cube[i][5].posX = -680;
		cube[i][5].posY = -600;
		cube[i][5].posZ = 0;

		cube[i][6].scaleX = 0.25;
		cube[i][6].scaleY = 2;
		cube[i][6].scaleZ = 1;
		cube[i][6].posX = -2467;
		cube[i][6].posY = -60;
		cube[i][6].posZ = 0;
	}
	glutMainLoop();
}

int angle1 = 30;
int angle2 = 30;
bool flag = true;
int flagCount = 0;

GLvoid drawScene()
{
	//glMatrixMode(GL_PROJECTION);
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(5.0);
	glLineWidth(1.0);
	//DrawLine();


	TIME();


	glColor3f(0, 0, 1);
	for (int j = 0; j < 6; ++j)
	{
		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);
		glTranslated(300 * j, 0, 0);

		glColor3f((float)rand()/RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
		for (int i = 0; i < 7; ++i)
		{
			if (cube[j][i].flag == 0) {
				glPushMatrix();
				glScalef(cube[j][i].scaleX, cube[j][i].scaleY, cube[j][i].scaleZ);
				glTranslatef(cube[j][i].posX, cube[j][i].posY, cube[j][i].posZ);
				glutWireCube(100);
				glPopMatrix();
			}
			else if (cube[j][i].flag == 1) {
				glPushMatrix();
				glScalef(cube[j][i].scaleX, cube[j][i].scaleY, cube[j][i].scaleZ);
				glTranslatef(cube[j][i].posX, cube[j][i].posY, cube[j][i].posZ);
				glutSolidCube(100);
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	if (flagCount > 1000)
	{
		if (flag == true)
			flag = false;
		else if (flag == false)
			flag = true;

		flagCount = 0;
	}

	if (!flag)
	{
		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(-377, 100, 0);
		glutWireCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(-377, -100, 0);
		glutWireCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(20, -100, 0);
		glutWireCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(20, 100, 0);
		glutWireCube(25);

		glPopMatrix();
	}
	else if (flag)
	{
		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(-377, 100, 0);
		glutSolidCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(-377, -100, 0);
		glutSolidCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(20, -100, 0);
		glutSolidCube(25);

		glPopMatrix();

		glPushMatrix();
		glRotatef(angle1, 1, 0, 0);
		glRotatef(angle2, 0, 1, 0);

		glTranslatef(20, 100, 0);
		glutSolidCube(25);

		glPopMatrix();
	}
	std::cout << flagCount << std::endl;
	glutSwapBuffers();
}

GLvoid reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(-800, 800, -800, 800, -800, 800);

}

bool left_button;

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		angle1--;
		break;
	case 's':
		angle1++;
		break;
	case 'd':
		angle2++;
		break;
	case 'a':
		angle2--;
		break;
	}
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{

	}
}

void Mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}

	left_button = true;
}

void Motion(int x, int y)
{
	if (left_button == true)
	{

	}

}

void TimerFunction(int value)
{

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	cube[0][0].time = (tm.tm_hour / 10); //hour 10의자리
	cube[1][0].time = (tm.tm_hour % 10); //hour 1의 자리

	cube[2][0].time = (tm.tm_min / 10); //hour 1의 자리
	cube[3][0].time = (tm.tm_min % 10); //hour 1의 자리

	cube[4][0].time = (tm.tm_sec / 10); //sec 1의자리
	cube[5][0].time = (tm.tm_sec % 10); //sec 10의자리

	flagCount += 20;


	glutPostRedisplay(); //화면 재출력
	glutTimerFunc(10, TimerFunction, 1);
}

void TIME()
{
	system("cls");
	printf("%d", cube[5][0].time);

	/*  3
	0 2
	1
	4 6
	5
	*/
	for (int i = 0; i < 6; ++i) {
		if (cube[i][0].time == 0)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 0;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 1;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}

		if (cube[i][0].time == 1)
		{
			cube[i][0].flag = 0;
			cube[i][1].flag = 0;
			cube[i][2].flag = 1;
			cube[i][3].flag = 0;
			cube[i][4].flag = 0;
			cube[i][5].flag = 0;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 2)
		{
			cube[i][0].flag = 0;
			cube[i][1].flag = 1;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 1;
			cube[i][5].flag = 1;
			cube[i][6].flag = 0;
		}
		if (cube[i][0].time == 3)
		{
			cube[i][0].flag = 0;
			cube[i][1].flag = 1;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 0;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 4)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 1;
			cube[i][2].flag = 1;
			cube[i][3].flag = 0;
			cube[i][4].flag = 0;
			cube[i][5].flag = 0;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 5)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 1;
			cube[i][2].flag = 0;
			cube[i][3].flag = 1;
			cube[i][4].flag = 0;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 6)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 1;
			cube[i][2].flag = 0;
			cube[i][3].flag = 1;
			cube[i][4].flag = 1;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 7)
		{
			cube[i][0].flag = 0;
			cube[i][1].flag = 0;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 0;
			cube[i][5].flag = 0;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 8)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 1;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 1;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}
		if (cube[i][0].time == 9)
		{
			cube[i][0].flag = 1;
			cube[i][1].flag = 1;
			cube[i][2].flag = 1;
			cube[i][3].flag = 1;
			cube[i][4].flag = 0;
			cube[i][5].flag = 1;
			cube[i][6].flag = 1;
		}
	}
}
