#include <random>
#include "Object.h"
#include "ProjectDefines.h"

/* 알파 블렌딩 선형보간 */
GLenum Src = GL_SRC_ALPHA;
GLenum Dest = GL_ONE_MINUS_SRC_ALPHA;

void Object::getPos(myVector4f * pos)
{
	*pos = Object::pos;
}

void Object::setPos(const myVector4f & pos)
{
	Object::pos = pos;
}

void Object::getVel(myVector4f * vel)
{
	*vel = Object::vel;
}

void Object::setVel(const myVector4f & vel)
{
	Object::vel = vel;
}

void Object::getAccel(myVector4f * accel)
{
	*accel = Object::accel;
}

void Object::setAccel(const myVector4f & accel)
{
	Object::accel = accel;
}

void Object::getColor(myVector4f * rgba)
{
	*rgba = Object::color;
}

void Object::setColor(const myVector4f & rgba)
{
	Object::color = rgba;
}

void Object::getMass(GLfloat * mass)
{
	*mass = Object::mass;
}

void Object::setMass(GLfloat mass)
{
	Object::mass = mass;
}

GLfloat Object::getAngle()
{
	return Object::angle;
}

void Object::setAngle(GLfloat degree)
{
	Object::angle = degree;
}

int Object::getShape()
{
	return shape;
}

void Object::setShape(int shape)
{
	Object::shape = shape;
}

int Object::getState()
{
	return state;
}

void Object::setState(int state)
{
	Object::state = state;
}

bool Object::getLifeSign()
{
	return isAlive;
}

void Object::setLifeSign(bool truefalse)
{
	Object::isAlive = truefalse;
}

bool Object::getMouseClicked()
{
	return isClicked;
}

void Object::setMouseClicked(bool truefalse)
{
	Object::isClicked = truefalse;
}

void Object::setDestination(const myVector4f& dest)
{
	destination = dest;
}

void Object::setSize(GLfloat size)
{
	Object::size = size;
}

GLfloat Object::getSize()
{
	return size;
}

void Object::initiate()
{
	if (shape == RECTANGLE || shape == DIAMOND) {
		if (pos.y > MAX_WINDOW_HEIGHT + rectEdge) {
			pos.y = MIN_WINDOW_HEIGHT - rectEdge;
			color.a = 1.f;
			shape = rand() % 2 + 1;
			isAlive = true;
		}
	}
	if (shape == TRIANGLE && state == STATIC) {
		if (pos.x > MAX_WINDOW_WIDTH + rectEdge) {
			pos.x = MIN_WINDOW_WIDTH - rectEdge;
			angle = static_cast<GLfloat>(rand() % 360);
			isAlive = true;
		}
	}
}

void Object::update(float eTime)
{
	if (shape == RECTANGLE || shape == DIAMOND) {
		initiate();
		if (pos.y > MAX_WINDOW_HEIGHT - 3.f * rectEdge) {
			glBlendFunc(Src, Dest);
			if (color.a > 0) {
				color.a -= 1.f * eTime;
			}
		}
	}

	if (shape == TRIANGLE && state == STATIC) {
		initiate();
		angle += 100.f * eTime;
	}

	if (shape == TRIANGLE && state == DYNAMIC) {
		if (pos.y < MIN_WINDOW_HEIGHT) {
			myVector4f tmp(0.f,0.f,0.f);
			accel = tmp;
			vel = tmp;
			state = STATIC;
			isAlive = false;
		}
	}

	if (shape == STAR && isAlive) {
		myVector4f tmpColor((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
		color = tmpColor;
		angle -= 200.f * eTime;
		if (0.5f > getDistance(destination, pos)) {
			myVector4f tmpVel(0.f, 0.f, 0.f);
			vel = tmpVel;
		}
	}


	/* accelation update */
	if (shape == TRIANGLE && state == DYNAMIC && isAlive) { accel = accel - gravity; }
	/* velocity update */
	vel = vel + accel * eTime;
	/* position update */
	pos = pos + vel * eTime;

}

void Object::drawShape()
{
	if (shape == NONE) return;
	if (isAlive) {
		if (shape == TRIANGLE) {
			glBegin(GL_TRIANGLES);
			glColor4f(color.r, color.g, color.b, color.a);
			glVertex3f(-rectEdgeHalf, rectEdgeHalf, 0.f);
			glVertex3f(-rectEdgeHalf, -rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, -rectEdgeHalf, 0.f);
			glEnd();
		}
		else if (shape == RECTANGLE) {
			glColor4f(color.r, color.g, color.b, color.a);
			glBegin(GL_POLYGON);
			glVertex3f(-rectEdgeHalf, rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, -rectEdgeHalf, 0.f);
			glVertex3f(-rectEdgeHalf, -rectEdgeHalf, 0.f);
			glEnd();
		}
		else if (shape == DIAMOND) {
			glPushMatrix();
			glRotatef(45.f, 0.f, 0.f, 1.f);
			glColor4f(color.r, color.g, color.b, color.a);
			glBegin(GL_POLYGON);
			glVertex3f(-rectEdgeHalf, rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, -rectEdgeHalf, 0.f);
			glVertex3f(-rectEdgeHalf, -rectEdgeHalf, 0.f);
			glEnd();
			glPopMatrix();
		}
		else if (shape == STAR) {
			glPushMatrix();
			glColor4f(color.r, color.g, color.b, color.a);
			glBegin(GL_POLYGON);
			glVertex3f(0.f, rectEdgeHalf, 0.f);
			glVertex3f(-rectEdgeHalf, -rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, -rectEdgeHalf, 0.f);
			glEnd();
			glTranslatef(0.f, -rectEdgeHalf * 2.f / 3.f, 0.f);
			glRotatef(180.f, 0.f, 0.f, 1.f);
			glBegin(GL_POLYGON);
			glVertex3f(0.f, rectEdgeHalf, 0.f);
			glVertex3f(-rectEdgeHalf, -rectEdgeHalf, 0.f);
			glVertex3f(rectEdgeHalf, -rectEdgeHalf, 0.f);
			glEnd();
			glPopMatrix();
		}
	}
}

bool Object::collideCheck()
{
	return false;
}
