#pragma once
/* �ۼ��� - 2013182024 KPU SMS, helped by LSK 

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
	{	/* ���� ����ü, ������ ���� �پ缺 ����� ���ؼ�, ����� - �̸����� ����ü ���� ���� ��� */
		GLfloat m[4];					//	��İ� ����
		struct { GLfloat x, y, z, w; };	//	�Ϲ� ��������
		struct { GLfloat r, g, b, a; };	//	rgba ��������
	};

	myVector4f() : x{ 0.f }, y{ 0.f }, z{ 0.f }, w{ 1.f } {}
	myVector4f(double x, double y, double z);
	myVector4f(int x, int y, int z);
	myVector4f(float x, float y, float z);
	myVector4f(const myVector4f& other);
	~myVector4f();

	/* ������ �����ε� */
		/* ��Į��� */
	myVector4f operator*(float scalar);
	myVector4f operator*(int scalar);
	myVector4f operator*(double scalar);
	myVector4f operator/(float scalar);
	myVector4f operator/(int scalar);
	myVector4f operator/(double scalar);

		/* ���� ���� ���� */
	myVector4f operator-() const;

		/* ���� ���� */
	myVector4f operator+(const myVector4f& rvalue);

		/* ���� ���� */
	myVector4f operator-(const myVector4f& rvalue);

		/* ���� ���� */
	myVector4f& operator=(const myVector4f& lvalue);

		/* �ݺ����� ���� */
	myVector4f& operator+=(myVector4f& lvalue);

		
	/* ! ���� ��ǥ���� �ʿ伺�� ���� ����غ��� */
	/* ������ �����ε�? +, - �ΰ� ������? */
	/* ���͸� ���� �������ұ�? ���������� ���, Ŭ������ �ؾ��ұ�? ����ü�� �� �ȵ���?�� ���� ��� */
};

myVector4f operator*(int scalar, const myVector4f& rhsVector);
myVector4f operator*(float scalar, const myVector4f& rhsVector);
myVector4f operator*(double scalar, const myVector4f& rhsVector);

float dotproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);
myVector4f crossproduct(const myVector4f& lhsVector, const myVector4f& rhsVector);
myVector4f normalize(const myVector4f& lvalue);
float getNorm(const myVector4f& vector);
float getDistance(const myVector4f& vector1, const myVector4f& vector2);
float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2); /* sqrtf ������ ��� �����̿���, ��Ʈ���� ���� �������¸� �����մϴ�. */

const myVector4f gravity{ 0.f, 0.49f, 0.f };	/* y�� �߷º���, 2���� test���Դϴ�. */
const myVector4f zeroVector;					/* ������ */