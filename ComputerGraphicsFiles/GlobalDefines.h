#pragma once
#include <cstdio>
#include <random>
#include <ctime>

/* 상기 왈 : 요즘 constexpr을 밀고 있다. #define 사용 대신에. */
/* 사용 연습을 해봐야겠다. 무슨 차인지도 알아보면 좋을 것 같다. */

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
/* 걍 편하게 쓰려고. 최대한 main문 비워보자. */
void CB_timerfunc(int v);	/* elapsedTime(aka time) 쓸꺼면 굳이 필요할까 의문이 듬 */
void CB_mousemovefunc(int button, int state, int x, int y);
void CB_keydownfunc(unsigned char key, int x, int y);
void CB_keyupfunc(unsigned char key, int x, int y);
void CB_specialkeydownfunc(int key, int x, int y);	/* GL에서 Arrow키 하려면 specialkeyfunc에 */
void CB_specialkeyupfunc(int key, int x, int y);
void CB_mousemotionfunc(int x, int y);
void CB_idle();	/* idle 상태 */
GLvoid ReShape(int w, int h);
GLvoid RenderScene(GLvoid);
