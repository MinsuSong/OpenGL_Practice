#pragma once
#include <gl/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <float.h>

#define MY_WINDOW_WIDTH		800.f
#define MY_WINDOW_HEIGHT	600.f

#define MAX_WINDOW_X		800.f
#define MIN_WINDOW_X		0.f

#define MAX_WINDOW_Y		600.f
#define MIN_WINDOW_Y		0.f

#define MAX_WINDOW_Z		400.f
#define MIN_WINDOW_Z		-400.f

#define GROUND_SIZE			300.f

#define PI 3.1415926535f

struct Vector {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

GLfloat dotproduct(Vector a, Vector b);
GLvoid normalize(Vector* v);
GLfloat getNorm(Vector v);
GLfloat getDistance(Vector a, Vector b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void timerfunction(int value);
void mousemovefunction(int button, int state, int x, int y);
void keydownfunction(unsigned char key, int x, int y);

/*
	<<	앞으로의 실습 요구사항	>>
	
	1.	더블 버퍼링 적용 (실습 11번부터 적용 시작)
			glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
			함수 드로잉 부분에서 glutSwapBuffers(); 호출

	2.	각 축에 대한 회전 및 이동 기능 (별도의 언급 없어도 가능하도록, 마찬가지로 실습 11번부터 적용 시작) 

			모든 실습은 glOrtho(0.0, 800.0, 0.0, 600.0, -400.0, 400.0); 로 z축 공간을 부여하도록 한다.
*/