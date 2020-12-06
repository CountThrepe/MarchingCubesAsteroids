#include "Point.hpp"

const int nVerts = 12;
const int nTris = 20;

vtx verts[nVerts] = {
    { 0.000, 0.000, 1.000}, { 0.894, 0.000, 0.447}, { 0.276, 0.851, 0.447},
    {-0.724, 0.526, 0.447}, {-0.724,-0.526, 0.447}, { 0.276,-0.851, 0.447},
    { 0.724, 0.526,-0.447}, {-0.276, 0.851,-0.447}, {-0.894, 0.000,-0.447},
    {-0.276,-0.851,-0.447}, { 0.724,-0.526,-0.447}, { 0.000, 0.000,-1.000}
};

tri tris[nTris] = {
    { 2, 1, 0}, { 3, 2, 0}, { 4, 3, 0}, { 5, 4, 0}, { 1, 5, 0},
    {11, 6, 7}, {11, 7, 8}, {11, 8, 9}, {11, 9,10}, {11,10, 6},
    { 1, 2, 6}, { 2, 3, 7}, { 3, 4, 8}, { 4, 5, 9}, { 5, 1,10},
    { 2, 7, 6}, { 3, 8, 7}, { 4, 9, 8}, { 5,10, 9}, { 1, 6,10}
};

Mesh mesh(nVerts, verts, verts, NULL, nTris, tris);

Point::Point(float pX, float pY, float pZ, float r, float val) : Object(pX, pY, pZ, 0, 0, 0, r, r, r) {
    this->radius = r;
    this->value = val;
}

Point::Point() : Object() {
    this->radius = 1;
    this->value = 1;
}

Point::~Point() {}

void Point::render() { // Icosasphere rendering taken from example 13, slightly modified
    // Set material properties
    float black[]  = {0.0,0.0,0.0,1.0};
    float purple[]  = {1.0,0.0,0.5,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,purple);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);

    // Save transformation
    glPushMatrix();

    // Offset and scale
    glTranslatef(pX, pY, pZ);
    glRotatef(rX, 1, 0, 0);
    glRotatef(rY, 0, 1, 0);
    glRotatef(rZ, 0, 0, 1);
    glScalef(sX,sY,sZ);

    mesh.render();

    glPopMatrix();
}

float Point::getRadius() {
    return this->radius;
}

void Point::setRadius(float val) {
    if(val < 0) val = 0;
    this->radius = val;
    this->setScale(val, val, val);
}

float Point::getValue() {
    return this->value;
}

void Point::setValue(float val) {
    if(val > 1) val = 1;
    else if(val < 0) val = 0;
    this->value = val;
}