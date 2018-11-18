#include "CGMath.h"

myVector4f::myVector4f(double x, double y, double z)
{
	myVector4f::x = static_cast<GLfloat>(x);
	myVector4f::y = static_cast<GLfloat>(y);
	myVector4f::z = static_cast<GLfloat>(z);
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(int x, int y, int z)
{
	myVector4f::x = static_cast<GLfloat>(x);
	myVector4f::y = static_cast<GLfloat>(y);
	myVector4f::z = static_cast<GLfloat>(z);
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(float x, float y, float z)
{
	myVector4f::x = x;
	myVector4f::y = y;
	myVector4f::z = z;
	myVector4f::w = 1.f;
}

myVector4f::myVector4f(const myVector4f & other)
{
	memcpy_s(this, sizeof(myVector4f), &other, sizeof(myVector4f));
}

myVector4f::~myVector4f()
{
}

myVector4f myVector4f::operator*(float scalar)
{
	myVector4f tmp(scalar * x, scalar * y, scalar * z);
	return tmp;
}

myVector4f myVector4f::operator*(int scalar)
{
	myVector4f tmp(static_cast<float>(scalar) * x, static_cast<float>(scalar) * y, static_cast<float>(scalar) * z);
	return tmp;
}

myVector4f myVector4f::operator*(double scalar)
{
	myVector4f tmp(static_cast<float>(scalar) * x, static_cast<float>(scalar) * y, static_cast<float>(scalar) * z);
	return tmp;
}

myVector4f myVector4f::operator/(float scalar)
{
	myVector4f tmp(x / scalar, y / scalar, z / scalar);
	return tmp;
}

myVector4f myVector4f::operator/(int scalar)
{
	myVector4f tmp(x / static_cast<float>(scalar), y / static_cast<float>(scalar), z / static_cast<float>(scalar));
	return tmp;
}

myVector4f myVector4f::operator/(double scalar)
{
	myVector4f tmp(x / static_cast<float>(scalar), y / static_cast<float>(scalar), z / static_cast<float>(scalar));
	return tmp;
}

myVector4f myVector4f::operator-() const
{
	myVector4f tmp(-x, -y, -z);
	return tmp;
}

myVector4f myVector4f::operator+(const myVector4f & rvalue)
{
	myVector4f tmp(rvalue.x + x, rvalue.y + y, rvalue.z + z);
	return tmp;
}

myVector4f myVector4f::operator-(const myVector4f & rvalue)
{
	myVector4f tmp(x - rvalue.x, y - rvalue.y, z - rvalue.z);
	return tmp;
}

myVector4f& myVector4f::operator=(const myVector4f & lvalue)
{
	memcpy_s(this, sizeof(myVector4f), &lvalue, sizeof(myVector4f));
	return *this;
}

myVector4f & myVector4f::operator+=(myVector4f & lvalue)
{
	*this = lvalue + *this;
	return *this;
}

bool isZero(float number)
{
	return (-FLT_EPSILON < number && number < FLT_EPSILON);
}

bool isEqual(float lhs, float rhs)
{
	return (isZero(lhs - rhs));
}

myVector4f operator*(int scalar, const myVector4f & rhsVector)
{
	myVector4f tmp(static_cast<float>(scalar) * rhsVector.x, static_cast<float>(scalar) * rhsVector.y, static_cast<float>(scalar) * rhsVector.z);
	return tmp;
}

myVector4f operator*(float scalar, const myVector4f & rhsVector)
{
	myVector4f tmp(scalar * rhsVector.x, scalar * rhsVector.y, scalar * rhsVector.z);
	return tmp;
}

myVector4f operator*(double scalar, const myVector4f & rhsVector)
{
	myVector4f tmp(static_cast<float>(scalar) * rhsVector.x, static_cast<float>(scalar) * rhsVector.y, static_cast<float>(scalar) * rhsVector.z);
	return tmp;
}

float dotproduct(const myVector4f & lhsVector, const myVector4f & rhsVector)
{
	return lhsVector.x * rhsVector.x + lhsVector.y * rhsVector.y + lhsVector.z * rhsVector.z;
}

myVector4f crossproduct(const myVector4f & lhsVector, const myVector4f & rhsVector)
{
	myVector4f tmp(
		lhsVector.y * rhsVector.z - lhsVector.z * rhsVector.y,
		lhsVector.z * rhsVector.x - lhsVector.x * rhsVector.z,
		lhsVector.x * rhsVector.y - lhsVector.y * rhsVector.x);
	return tmp;
}

myVector4f normalize(const myVector4f & lvalue)
{
	GLfloat norm = getNorm(lvalue);
	myVector4f tmp(lvalue.x / norm, lvalue.y / norm, lvalue.z / norm);
	return tmp;
}

float getNorm(const myVector4f & vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float getDistance(const myVector4f & vector1, const myVector4f & vector2)
{
	return sqrtf(
		(vector2.x - vector1.x) * (vector2.x - vector1.x) +
		(vector2.y - vector1.y) * (vector2.y - vector1.y) +
		(vector2.z - vector1.z) * (vector2.z - vector1.z)
	);
}

float getRoughMagnitude(const myVector4f & vector1, const myVector4f & vector2)
{
	return
		(vector2.x - vector1.x) * (vector2.x - vector1.x) +
		(vector2.y - vector1.y) * (vector2.y - vector1.y) +
		(vector2.z - vector1.z) * (vector2.z - vector1.z);
}