#include "myHeader.h"

Vector xAxis_UV{ 1.f,0.f,0.f };	//	xÃà ¹æÇâ º¤ÅÍ
Vector yAxis_UV{ 0.f,1.f,0.f };	//	yÃà ¹æÇâ º¤ÅÍ
Vector zAxis_UV{ 0.f,0.f,1.f };	//	zÃà ¹æÇâ º¤ÅÍ

float dotproduct(Vector a, Vector b)
{
	/* a * b * cos(theta) */
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void normalize(Vector* v)
{
	GLfloat tempNorm = getNorm(*v);
	v->x = (GLfloat)(v->x / tempNorm);
	v->y = (GLfloat)(v->y / tempNorm);
	v->z = (GLfloat)(v->z / tempNorm);
}

float getNorm(Vector v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float getDistance(Vector a, Vector b)
{
	return sqrtf( (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z) );
}