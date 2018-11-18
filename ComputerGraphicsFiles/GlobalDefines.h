#pragma once
#include <cstdio>
#include <random>
#include <ctime>

/* ��� �� : ���� constexpr�� �а� �ִ�. #define ��� ��ſ�. */
/* ��� ������ �غ��߰ڴ�. ���� �������� �˾ƺ��� ���� �� ����. */

#define MY_WINDOW_WIDTH		800.f
#define MAX_WINDOW_WIDTH	MY_WINDOW_WIDTH / 2.f
#define MIN_WINDOW_WIDTH	-MAX_WINDOW_WIDTH

#define MY_WINDOW_HEIGHT	600.f
#define MAX_WINDOW_HEIGHT	MY_WINDOW_HEIGHT / 2.f
#define MIN_WINDOW_HEIGHT	-MAX_WINDOW_HEIGHT

#define MY_WINDOW_DEPTH		600.f
#define MAX_WINDOW_DEPTH	MY_WINDOW_DEPTH / 2.f
#define MIN_WINDOW_DEPTH	-MAX_WINDOW_DEPTH

#define DEFAULT_GROUND_SIZE	300.f

/* declaration of callback functions */
/* �� ���ϰ� ������. �ִ��� main�� �������. */
void CB_timerfunc(int v);	/* elapsedTime(aka time) ������ ���� �ʿ��ұ� �ǹ��� �� */
void CB_mousemovefunc(int button, int state, int x, int y);
void CB_keydownfunc(unsigned char key, int x, int y);
void CB_keyupfunc(unsigned char key, int x, int y);
void CB_specialkeydownfunc(int key, int x, int y);	/* GL���� ArrowŰ �Ϸ��� specialkeyfunc�� */
void CB_specialkeyupfunc(int key, int x, int y);
void CB_mousemotionfunc(int x, int y);
void CB_idle();	/* idle ���� */
GLvoid ReShape(int w, int h);
GLvoid RenderScene(GLvoid);
