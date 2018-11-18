#pragma once
#include "CGMath.h"
#include "GlobalDefines.h"
#include "ProjectDefines.h"

/* for 숙제 1 */
constexpr int NONE{ 0 };
constexpr int RECTANGLE{ 1 };
constexpr int DIAMOND{ 2 };
constexpr int TRIANGLE{ 3 };
constexpr int CIRCLE{ 4 };
constexpr int STAR{ 5 };

constexpr int STATIC{ 0 };
constexpr int DYNAMIC{ 1 };

constexpr float rectEdgeHalf{ 15.0f };	/* rectangle's Edge's half, 삼각형의 경우는 외접사각형 */
constexpr float rectEdge{ 2.f * rectEdgeHalf };	/* rectangle's Edge */

//constexpr

class Object {
private:
	myVector4f pos;
	myVector4f vel;
	myVector4f accel;
	myVector4f color;
	GLfloat mass{ 0.f };		/* 폐기, 힘으로 물리 적용할 꺼면 다시 부활시켜야함 */
	GLfloat angle{ 0.f };		/* z축 회전, degree */
	int shape{ NONE };			/* 도형의 모양 */
	int state{ STATIC };		/* 상태 - STATIC(고정), DYNAMIC(중력 받음)*/
	myVector4f destination;
	GLfloat size{ 1.f };
public:

	bool isAlive{ true };		/* Render 여부 */
	bool isClicked{ false };	/* MouseClick, 클릭 시 마우스에 렌더링*/

	void getPos(myVector4f* pos);
	void setPos(const myVector4f& pos);

	void getVel(myVector4f* vel);
	void setVel(const myVector4f& vel);

	void getAccel(myVector4f* accel);
	void setAccel(const myVector4f& accel);

	void getColor(myVector4f* rgba);
	void setColor(const myVector4f& rgba);

	void getMass(GLfloat* mass);
	void setMass(GLfloat mass);

	GLfloat getAngle();
	void setAngle(GLfloat degree);

	int getShape();
	void setShape(int shape);

	int getState();
	void setState(int state);

	bool getLifeSign();
	void setLifeSign(bool truefalse);

	bool getMouseClicked();
	void setMouseClicked(bool truefalse);

	void setDestination(const myVector4f& dest);

	void setSize(GLfloat size);
	GLfloat getSize();

	void initiate();
	void update(float eTime);
	//void addForce(const myVector4f& force, float eTime); /* 일단 폐기 */
	void drawShape();
	void render();
	bool collideCheck();
};