#ifndef _MESH_H
#define _MESH_H

#include <GL/glut.h>
#include <GL/glext.h>

struct vtx {
	float x,y,z;

	vtx operator+(const vtx& v) const {
		return {x+v.x, y+v.y, z+v.z};
	}

	vtx operator-(const vtx& v) const {
		return {x-v.x, y-v.y, z-v.z};
	}

	bool operator==(const vtx& v) {
		return v.x == x && v.y == y && v.z == z;
	}
};
struct tri {int A,B,C;};

class Mesh {
public:
	Mesh(int, vtx*, vtx*, vtx*, int, tri*);
	~Mesh();
	void render();
	void setTexture(unsigned int);

private:
	int numVerts, numTris;
	vtx* vertices;
    vtx* normals;
	tri* triangles;

	unsigned int textureID;
    vtx* textureCoordinates;
};

#endif