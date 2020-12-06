#ifndef _SPHERE_H
#define _SPHERE_H

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Object.hpp"
#include "Mesh.hpp"

class Point : public Object {
public:
	Point(float, float, float, float, float);
	Point();
	~Point();
	void render();
	float getRadius();
	void setRadius(float);
	float getValue();
	void setValue(float);

private:
	float radius;
	float value;

	void vertex(float, float);
};

#endif