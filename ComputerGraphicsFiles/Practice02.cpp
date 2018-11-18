#include <gl/freeglut.h>
#include <time.h>
#include <stdio.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);


void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Practice02");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	int column = rand() % 10 + 1;
	int row = rand() % 100 + 1;
	for (int j = 0; j < column; ++j) {
		double r = (double)rand() / RAND_MAX;
		double g = (double)rand() / RAND_MAX;
		double b = (double)rand() / RAND_MAX;
		for (int i = 0; i < row; ++i) {
			glColor3f(r + 0.025 * i, g + 0.025 * i, b + 0.025 * i);
			glRectf(800 / column * j, 600 / row * i, 800 / column * (j + 1), 600 / row * (i + 1));
		}
	}
	/*
	좌하단을 0,0으로 잡은 1사분면
	좌에서 우로, 하에서 상의 순서로 사각형 그림.

	1.0f에 가까워지도록 더해서 밝아지는 색으로 그라데이션 입힘.
	*/
	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, 800, 0, 600.0, -1.0, 1.0 );
}