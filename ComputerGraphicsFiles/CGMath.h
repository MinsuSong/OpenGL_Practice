#pragma once
/* 작성자 - 2013182024 KPU SMS, helped by LSK 

last write date : 2018. 11. 09 */
#include <gl/freeglut.h>
#include <math.h>
#include <float.h>

#define PI_d	3.1415926535
#define PI		3.1415926535f

constexpr GLfloat convert_Rad2Degree{ 180.f / PI };
constexpr GLfloat convert_Deg2Radian{ PI / 180.f };

bool isZero(float number);
bool isEqual(float lhs, float rhs);

class myVector4f {
public:
	union 
	{	/* 공용 구조체, 데이터 접근 다양성 향상을 위해서, 경고문구 - 이름없는 구조체 선언에 대한 경고 */
		GLfloat m[4];					//	행렬곱 연산
		struct { GLfloat x, y, z, w; };	//	일반 벡터형태
		struct { GLfloat r, g, b, a; };	//	rgba 벡터형태
	};

	myVector4f() : x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 1.f } {}
	myVector4f(double x, double y, double z);
	myVector4f(int x, int y, int z);
	myVector4f(float x, float y, float z);
	myVector4f(const myVector4f& other);
	~myVector4f();

	/* 연산자 오버로딩 */
		/* 스칼라곱 */
	myVector4f operator*(float scalar);
	myVector4f operator*(int scalar);
	myVector4f operator*(double scalar);
	myVector4f operator/(float scalar);
	myVector4f operator/(int scalar);
	myVector4f operator/(double scalar);

		/* 벡터 방향 역전 */
	myVector4f operator-() const;

		/* 벡터 덧셈 */
	myVector4f operator+(const myVector4f& rvalue);

		/* 벡터 뺄셈 */
	myVector4f operator-(const myVector4f& rvalue);

		/* 대입 연산 */
	myVector4f& operator=(const myVector4f& lvalue);

		/* 반복대입 연산 */
	myVector4f& operator+=(myVector4f& lvalue);

		
	/* ! 동차 좌표계의 필요성에 대해 고민해보자 */
	/* 연산자 오버로딩? +, - 두개 정도만? */
	/* 벡터를 굳이 가려야할까? 형식지정자 사용, 클래스로 해야할까? 구조체는 왜 안되지?에 대해 고민 */
};

myVector4f operator*(int scalar, const myVector4f& rhsVector);
myVector4f operator*(float scalar, const myVector4f& rhsVector);
myVector4f operator*(double scalar, const myVector4f& rhsVector);

float dotproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);
myVector4f crossproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);
myVector4f normalize(const myVector4f& lvalue);
float getNorm(const myVector4f& vector);
float getDistance(const myVector4f& vector1, const myVector4f& vector2);
float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2); /* sqrtf 연산이 비싼 연산이여서, 루트적용 안한 제곱상태를 리턴합니다. */

const myVector4f gravity{ 0.f, 0.49f, 0.f };	/* y축 중력벡터, 2차원 test용입니다. */
const myVector4f zeroVector;					/* 영벡터 */