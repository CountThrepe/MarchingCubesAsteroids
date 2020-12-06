#include "Object.hpp"

Object::Object(float pX, float pY, float pZ, float rX, float rY, float rZ, float sX, float sY, float sZ) {
	this->pX = pX;
	this->pY = pY;
	this->pZ = pZ;
	this->rX = rX;
	this->rY = rY;
	this->rZ = rZ;
	this->sX = sX;
	this->sY = sY;
	this->sZ = sZ;
}

Object::Object() : Object(0, 0, 0, 0, 0, 0, 1, 1, 1) {}

Object::~Object() {}

float Object::getPosX() {return pX;}
float Object::getPosY() {return pY;}
float Object::getPosZ() {return pZ;}
void Object::setPos(float x, float y, float z) {
	pX = x;
	pY = y;
	pZ = z;
}
void Object::translate(float x, float y, float z) {
	pX += x;
	pY += y;
	pZ += z;
}

float Object::getRotX() {return rX;}
float Object::getRotY() {return rY;}
float Object::getRotZ() {return rZ;}
void Object::setRot(float x, float y, float z) {
	rX = x;
	rY = y;
	rZ = z;
}
void Object::rotate(float x, float y, float z) {
	rX += x;
	rY += y;
	rZ += z;
}

float Object::getScaleX() {return sX;}
float Object::getScaleY() {return sY;}
float Object::getScaleZ() {return sZ;}
void Object::setScale(float x, float y, float z) {
	sX = x > 0 ? x : 0;
	sY = y > 0 ? y : 0;
	sZ = z > 0 ? z : 0;
}