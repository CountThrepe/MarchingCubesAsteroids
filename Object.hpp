#ifndef _OBJECT_H
#define _OBJECT_H

class Object {
public:
	Object(float, float, float, float, float, float, float, float, float);
	Object();
	virtual ~Object();

	float getPosX();
	float getPosY();
	float getPosZ();
	void setPos(float, float, float);
	void translate(float, float, float);

	float getRotX();
	float getRotY();
	float getRotZ();
	void setRot(float, float, float);
	void rotate(float, float, float);

	float getScaleX();
	float getScaleY();
	float getScaleZ();
	void setScale(float, float, float);

protected:
	// Position variables
	float pX;
	float pY;
	float pZ;

	// Rotation variables
	float rX;
	float rY;
	float rZ;

	// Scale variables
	float sX;
	float sY;
	float sZ;
};

#endif