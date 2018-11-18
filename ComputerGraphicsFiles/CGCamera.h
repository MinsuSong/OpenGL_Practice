#pragma once
/* 작성자 - 2013182024 KPU SMS, helped by LSK

last write date : 2018. 11. 09 */
#include <gl/freeglut.h>
#include "CGMath.h"

constexpr float frustum_fovy{0.ㄹf};

constexpr float frustum_Zfar{ 0.f };
constexpr float frustum_Znear{ 0.f };
constexpr float frustum_aspect{ 0.f };


class Camera {
private:
	myVector4f eye;
	myVector4f at;
	myVector4f up;
public:
	Camera();
	~Camera();




	/* 개발 요구사항 - 자유시점 이동, 마우스 움직임에 따른 카메라 x,y축 회전 */
};