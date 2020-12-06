#include "Mesh.hpp"

Mesh::Mesh(int nVerts, vtx* verts, vtx* norms, vtx* texCoords, int nTris, tri* tris) {
    textureID = 0;

	numVerts = nVerts;
	numTris = nTris;

	vertices = new vtx[numVerts];
	normals = new vtx[numVerts];
	if(texCoords != NULL) textureCoordinates = new vtx[numVerts];
	else textureCoordinates = NULL;
	for(int i = 0; i < numVerts; i++) {
		vertices[i] = verts[i];
		normals[i] = norms[i];
		if(texCoords != NULL) textureCoordinates[i] = texCoords[i];
	}

	triangles = new tri[numTris];
	for(int i = 0; i < numTris; i++) {
		triangles[i] = tris[i];
	}

}

Mesh::~Mesh() {
	delete[] vertices;
    delete[] normals;
    delete[] textureCoordinates;
	delete[] triangles;
}

void Mesh::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    if(textureCoordinates != NULL && textureID != 0) {
        glTexCoordPointer(3, GL_FLOAT, 0, textureCoordinates);
        glEnable(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, textureID);
    }

    glDrawElements(GL_TRIANGLES, 3*numTris, GL_UNSIGNED_INT, triangles);

    glDisable(GL_TEXTURE_3D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::setTexture(unsigned int id) {
    textureID = id;
}