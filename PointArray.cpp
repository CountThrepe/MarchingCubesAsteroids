#include "PointArray.hpp"

PointArray::PointArray(const int xLen, const int yLen, const int zLen, float s, float t, bool b, bool i) {
	this->x = xLen;
	this->y = yLen;
	this->z = zLen;
	if(xLen >= yLen && xLen >= zLen) this->largest = xLen;
	else if(yLen >= xLen && yLen >= zLen) this->largest = yLen;
	else this->largest = zLen;

	this->offset = 0;
	this->noiseScale = s;
	this->threshold = t;
	this->vignette = b;
	this->interpolate = i;
	this->texture = true;

	this->needUpdate = false;

	// Allocate array space
	this->points = new Point***[xLen];
	for(int i = 0; i < xLen; i++) {
		this->points[i] = new Point**[yLen];
		for(int j = 0; j < yLen; j++) {
			this->points[i][j] = new Point*[zLen];
			for(int k = 0; k < zLen; k++) {
				this->points[i][j][k] = NULL;
			}
		}
	}

    texGen = new TextureGenerator(3, 128);
	texGen->setLayerColor(0, 0.5, 0.5, 0.5);
    texGen->setLayerColor(1, 1, 0.8, 0.8);
    texGen->setLayerColor(2, 0.8, 0.8, 1);

	mesh = NULL;
	noise = NULL;
	this->scramble();
	this->update();
}

PointArray::~PointArray() {
	for(int i = 0; i < this->x; i++) {
		for(int j = 0; j < this->y; j++) {
			for(int k = 0; k < this->z; k++) {
				delete this->points[i][j][k];
			}
			delete[] this->points[i][j];
		}
		delete[] this->points[i];
	}
	delete[] this->points;

	delete noise;
	delete mesh;
	delete texGen;
}

void PointArray::renderPoints() {
	for(int i = 0; i < this->x; i++) {
        for(int j = 0; j < this->y; j++) {
            for(int k = 0; k < this->z; k++) {
            	if(this->points[i][j][k]->getValue() > this->threshold) this->points[i][j][k]->render();
            }
        }
    }
}

void PointArray::renderMesh() {
	float black[]  = {0.0,0.0,0.0,1.0};
    float purple[]  = {1.0,0.0,0.5,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,purple);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,1);
    glColor3f(1,1,1);

    // Save transformation
    glPushMatrix();

    glTranslatef(0, 0, 0);
    glRotatef(0, 1, 0, 0);
    glRotatef(0, 0, 1, 0);
    glRotatef(0, 0, 0, 1);
    glScalef(1,1,1);

    mesh->render();

    glPopMatrix();
}

void PointArray::renderBoth() {
	renderPoints();
	renderMesh();
}

void PointArray::scramble() {
	delete noise;
	noise = new OpenSimplexNoise::Noise(time(NULL));

	texGen->scramble();
	texID = texGen->getTexture();

	this->needUpdate = true;
}

void PointArray::updateArray() {
	for(int i = 0; i < this->x; i++) {
        for(int j = 0; j < this->y; j++) {
            for(int k = 0; k < this->z; k++) {
                delete this->points[i][j][k];

                float val = (this->noise->eval((i * this->noiseScale) + sin(this->offset),
                                               (j * this->noiseScale) + cos(this->offset),
                                               (k * this->noiseScale) + sin(this->offset)) + 1) / 2;
                if(this->vignette) {
                    float multiplier = 8 * (i < (this->x/2) ? i : this->x-i-1) * (j < this->y/2 ? j : this->y-j-1)
                            * (k < this->z/2 ? k : this->z-k-1) / (float) ((this->x-1) * (this->y-1) * (this->z-1));
                    multiplier = pow(multiplier, 0.4);
                    val *= multiplier;
                }

                vtx v = indicesToVtx(i, j, k);
                this->points[i][j][k] = new Point(v.x, v.y, v.z, 0.05, val);
            }
        }
    }
}

void PointArray::updateAsteroid() {
	delete mesh;

	std::vector<tempTri> tempTris;
	std::vector<vtx> uniquePoints;

	for(int i = 0; i < this->x - 1; i++) {
		for(int j = 0; j < this->y - 1; j++) {
			for(int k = 0; k < this->z - 1; k++) {
				int points = 0;
				if(this->points[i][j][k]->getValue() > this->threshold) points |= 1;
				if(this->points[i+1][j][k]->getValue() > this->threshold) points |= 2;
				if(this->points[i+1][j][k+1]->getValue() > this->threshold) points |= 4;
				if(this->points[i][j][k+1]->getValue() > this->threshold) points |= 8;
				if(this->points[i][j+1][k]->getValue() > this->threshold) points |= 16;
				if(this->points[i+1][j+1][k]->getValue() > this->threshold) points |= 32;
				if(this->points[i+1][j+1][k+1]->getValue() > this->threshold) points |= 64;
				if(this->points[i][j+1][k+1]->getValue() > this->threshold) points |= 128;

				// Which edges on this cube are unique, so we know to add new points to the vector
				bool newEdge[] = {false,false,false,false,false,true,true,false,false,false,true,false};
				if(!i && !j) newEdge[3] = true;
				if(!i && !k) newEdge[8] = true;
				if(!j && !k) newEdge[0] = true;
				if(!i) {
					newEdge[7] = true;
					newEdge[11] = true;
				}
				if(!j) {
					newEdge[1] = true;
					newEdge[2] = true;
				}
				if(!k) {
					newEdge[4] = true;
					newEdge[9] = true;
				}

				// For each triangle to add
				int newPoints = 0;
				for(int tCount = 0; tCount < 5; tCount++) {
					tri t = triTable[points][tCount];
					if(t.A == -1) break;

					// Add triangle
					tempTri newTri = {};

					int edge = t.A;
					newTri.A = createTriVtx(i, j, k, edge);
					if(newEdge[edge] && !(newPoints&(1<<edge))) {
						uniquePoints.push_back(newTri.A);
						newPoints |= 1<<edge;
					}

					edge = t.B;
                    newTri.B = createTriVtx(i, j, k, edge);
					if(newEdge[edge] && !(newPoints&(1<<edge))) {
						uniquePoints.push_back(newTri.B);
						newPoints |= 1<<edge;
					}

					edge = t.C;
                    newTri.C = createTriVtx(i, j, k, edge);
					if(newEdge[edge] && !(newPoints&(1<<edge))) {
						uniquePoints.push_back(newTri.C);
						newPoints |= 1<<edge;
					}

					tempTris.push_back(newTri);
				}
			}
		}
	}

	const int nPoints = uniquePoints.size();
	const int nTriangles = tempTris.size();

	vtx pointArray[nPoints] = {};
	vtx normalArray[nPoints] = {};
	vtx texArray[nPoints] = {};
	tri triangleArray[nTriangles] = {};

	// For each point, 
	for(int p = 0; p < nPoints; p++) {
        vtx v = uniquePoints[p];
	    pointArray[p] = v;
        texArray[p] = vtxToTex(v);

		vtx normSum = {0,0,0};
		for(int t = 0; t < nTriangles; t++) {
			tempTri temp = tempTris[t];
			bool hasPoint = false;

			if(temp.A == v) {
				triangleArray[t].A = p;
				hasPoint = true;
			} else if(temp.B == v) {
				triangleArray[t].B = p;
				hasPoint = true;
			} else if(temp.C == v) {
				triangleArray[t].C = p;
				hasPoint = true;
			}

			// If this triangle has the current point, calculate it's normal and add it to the sum
			if(hasPoint) {
				vtx ba = temp.B - temp.A;
				vtx ca = temp.C - temp.A;
				vtx cross = {(ba.y*ca.z) - (ba.z*ca.y), (ba.z*ca.x) - (ba.x*ca.z), (ba.x*ca.y) - (ba.y*ca.x)};
				float len = sqrt((cross.x*cross.x) + (cross.y*cross.y) + (cross.z*cross.z));
				vtx norm = {cross.x / len, cross.y / len, cross.z / len};

				normSum = normSum + norm;
			}
		}

		normalArray[p] = normSum;
	}

	// Implementation pseudocode
	// Iterate through cubes, adding unique points and triangles to vectors
	// For each point in vector:
		// Add value to new array
		// For each triangle with that point in vector:
			// Add normal to sum for average
			// Update tri array with reference to point in point array
		// Update normal array with average normal
	// update mesh created with point, tri, and normal arrays

	mesh = new Mesh(nPoints, pointArray, normalArray, texArray, nTriangles, triangleArray);
    mesh->setTexture(texture ? texID : 0);
}

vtx PointArray::indicesToVtx(int i, int j, int k) {
	return indicesToVtx((float) i, (float) j, (float) k);
}

vtx PointArray::indicesToVtx(float i, float j, float k) {
    float denom = this->largest - 1;
    vtx v = {};
    v.x = (2 * i / denom) - (this->x / this->largest);
    v.y = (2 * j / denom) - (this->y / this->largest);
    v.z = (2 * k / denom) - (this->z / this->largest);
    return v;
}

vtx PointArray::vtxToTex(vtx v) {
    vtx tex = v;
    tex.x = (tex.x + 1) / 2;
    tex.y = (tex.y + 1) / 2;
    tex.z = (tex.z + 1) / 2;
    return tex;
}

vtx PointArray::createTriVtx(int i, int j, int k, int edge) {
    vtx pntOffset = edgePoints[edge];
    if(!this->interpolate) return indicesToVtx(i+pntOffset.x, j+pntOffset.y, k+pntOffset.z);

    vtx temp = {i+pntOffset.x, j+pntOffset.y, k+pntOffset.z};
    if (fmod(temp.x, 1)) {
        int i1 = (int) floor(temp.x), i2 = (int) ceil(temp.x);
        float val1 = this->points[i1][(int) temp.y][(int) temp.z]->getValue();
        float val2 = this->points[i2][(int) temp.y][(int) temp.z]->getValue();
        float offset = abs((this->threshold - val1) / (val2 - val1));
        return indicesToVtx(i1 + offset, temp.y, temp.z);
    } else if (fmod(temp.y, 1)) {
        int j1 = (int) floor(temp.y), j2 = (int) ceil(temp.y);
        float val1 = this->points[(int) temp.x][j1][(int) temp.z]->getValue();
        float val2 = this->points[(int) temp.x][j2][(int) temp.z]->getValue();
        float offset = abs((this->threshold - val1) / (val2 - val1));
        return indicesToVtx(temp.x, j1 + offset, temp.z);
    } else {
        int k1 = (int) floor(temp.z), k2 = (int) ceil(temp.z);
        float val1 = this->points[(int) temp.x][(int) temp.y][k1]->getValue();
        float val2 = this->points[(int) temp.x][(int) temp.y][k2]->getValue();
        float offset = abs((this->threshold - val1) / (val2 - val1));
        return indicesToVtx(temp.x, temp.y, k1 + offset);
    }
}

void PointArray::setDimensions(int xLen, int yLen, int zLen) {
    // Delete current point array
    for(int i = 0; i < this->x; i++) {
        for(int j = 0; j < this->y; j++) {
            for(int k = 0; k < this->z; k++) {
                delete this->points[i][j][k];
            }
            delete[] this->points[i][j];
        }
        delete[] this->points[i];
    }
    delete[] this->points;

    this->x = xLen;
    this->y = yLen;
    this->z = zLen;


    if(xLen >= yLen && xLen >= zLen) this->largest = xLen;
    else if(yLen >= xLen && yLen >= zLen) this->largest = yLen;
    else this->largest = zLen;

    // Allocate new array
    this->points = new Point***[xLen];
    for(int i = 0; i < xLen; i++) {
        this->points[i] = new Point**[yLen];
        for(int j = 0; j < yLen; j++) {
            this->points[i][j] = new Point*[zLen];
            for(int k = 0; k < zLen; k++) {
                this->points[i][j][k] = NULL;
            }
        }
    }

    this->needUpdate = true;
    this->update();
}

void PointArray::setScale(float s) {
	this->noiseScale = s;
    this->needUpdate = true;
}

void PointArray::setThreshold(float t) {
	this->threshold = t;
    this->needUpdate = true;
}

void PointArray::setVignette(bool val) {
    this->vignette = val;
    this->needUpdate = true;
}

void PointArray::setInterpolate(bool val) {
    this->interpolate = val;
    this->needUpdate = true;
}

void PointArray::setTexture(bool val) {
    this->texture = val;
    mesh->setTexture(val ? texID : 0);
}

void PointArray::incrementOffset() {
    this->offset += 0.01;
    if(this->offset > 2 * M_PI) this->offset -= 2 * M_PI;
    this->needUpdate = true;
}

void PointArray::update() {
    if(!this->needUpdate) return;
    updateArray();
    updateAsteroid();
    this->needUpdate = false;
}