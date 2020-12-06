#include "MarchingCubes.hpp"

Mesh* MarchingCubes::generateAsteroid(PointArray* pointArr, double threshold) {
	vector<tempTri> tempTris;
	std::vector<vtx> uniquePoints;

	for(int x = 0; x < dim - 1; x++) {
		for(int y = 0; y < dim - 1; y++) {
			for(int z = 0; z < dim - 1; z++) {
				char points = 0;
				if(pointArr->getPoint(x,y,z)->getValue() > threshold) points |= 1;
				if(pointArr->getPoint(x+1,y,z)->getValue() > threshold) points |= 2;
				if(pointArr->getPoint(x+1,y,z+1)->getValue() > threshold) points |= 4;
				if(pointArr->getPoint(x,y,z+1)->getValue() > threshold) points |= 8;
				if(pointArr->getPoint(x,y+1,z)->getValue() > threshold) points |= 16;
				if(pointArr->getPoint(x+1,y+1,z)->getValue() > threshold) points |= 32;
				if(pointArr->getPoint(x+1,y+1,z+1)->getValue() > threshold) points |= 64;
				if(pointArr->getPoint(x,y+1,z+1)->getValue() > threshold) points |= 128;

				char newPoints = 0;
				for(int i = 0; i < 5; i++) {
					tri t = edgeTable[points][i];
					if(t.A == -1) break;

					tempTri newTri;
					switch(t.A) {
						// Maybe new points, add to unique points vector
						case 5:
							{
								vtx pnt = {(float)(x+2),(float)(y+2),(float)(z+1.5)};
								newTri.A = pnt;
								if(!(newPoints&1)) {
									newPoints += 1;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 6:
							{
								vtx pnt = {(float)(x+1.5),(float)(y+2),(float)(z+2)};
								newTri.A = pnt;
								if(!(newPoints&2)) {
									newPoints += 2;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 10:
							{
								vtx pnt = {(float)(x+2),(float)(y+1.5),(float)(z+2)};
								newTri.A = pnt;
								if(!(newPoints&4)) {
									newPoints += 4;
									uniquePoints.push_back(pnt);
								}
								break;
							}

						// Old points
						case 0:
							newTri.A = {(float)(x+1.5),(float)(y+1),(float)(z+1)};
							break;
						case 1:
							newTri.A = {(float)(x+2),(float)(y+1),(float)(z+1.5)};
							break;
						case 2:
							newTri.A = {(float)(x+1.5),(float)(y+1),(float)(z+2)};
							break;
						case 3:
							newTri.A = {(float)(x+1),(float)(y+1),(float)(z+1.5)};
							break;
						case 4:
							newTri.A = {(float)(x+1.5),(float)(y+2),(float)(z+1)};
							break;
						case 7:
							newTri.A = {(float)(x+1),(float)(y+2),(float)(z+1.5)};
							break;
						case 8:
							newTri.A = {(float)(x+1),(float)(y+1.5),(float)(z+1)};
							break;
						case 9:
							newTri.A = {(float)(x+2),(float)(y+1.5),(float)(z+1)};
							break;
						case 11:
							newTri.A = {(float)(x+1),(float)(y+1.5),(float)(z+2)};
							break;
					}

					switch(t.B) {
						// Maybe new points, add to unique points vector
						case 5:
							{
								vtx pnt = {(float)(x+2),(float)(y+2),(float)(z+1.5)};
								newTri.B = pnt;
								if(!(newPoints&1)) {
									newPoints += 1;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 6:
							{
								vtx pnt = {(float)(x+1.5),(float)(y+2),(float)(z+2)};
								newTri.B = pnt;
								if(!(newPoints&2)) {
									newPoints += 2;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 10:
							{
								vtx pnt = {(float)(x+2),(float)(y+1.5),(float)(z+2)};
								newTri.B = pnt;
								if(!(newPoints&4)) {
									newPoints += 4;
									uniquePoints.push_back(pnt);
								}
								break;
							}

						// Old points
						case 0:
							newTri.B = {(float)(x+1.5),(float)(y+1),(float)(z+1)};
							break;
						case 1:
							newTri.B = {(float)(x+2),(float)(y+1),(float)(z+1.5)};
							break;
						case 2:
							newTri.B = {(float)(x+1.5),(float)(y+1),(float)(z+2)};
							break;
						case 3:
							newTri.B = {(float)(x+1),(float)(y+1),(float)(z+1.5)};
							break;
						case 4:
							newTri.B = {(float)(x+1.5),(float)(y+2),(float)(z+1)};
							break;
						case 7:
							newTri.B = {(float)(x+1),(float)(y+2),(float)(z+1.5)};
							break;
						case 8:
							newTri.B = {(float)(x+1),(float)(y+1.5),(float)(z+1)};
							break;
						case 9:
							newTri.B = {(float)(x+2),(float)(y+1.5),(float)(z+1)};
							break;
						case 11:
							newTri.B = {(float)(x+1),(float)(y+1.5),(float)(z+2)};
							break;
					}

					switch(t.C) {
						// Maybe new points, add to unique points vector
						case 5:
							{
								vtx pnt = {(float)(x+2),(float)(y+2),(float)(z+1.5)};
								newTri.C = pnt;
								if(!(newPoints&1)) {
									newPoints += 1;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 6:
							{
								vtx pnt = {(float)(x+1.5),(float)(y+2),(float)(z+2)};
								newTri.C = pnt;
								if(!(newPoints&2)) {
									newPoints += 2;
									uniquePoints.push_back(pnt);
								}
								break;
							}
						case 10:
							{
								vtx pnt = {(float)(x+2),(float)(y+1.5),(float)(z+2)};
								newTri.C = pnt;
								if(!(newPoints&4)) {
									newPoints += 4;
									uniquePoints.push_back(pnt);
								}
								break;
							}

						// Old points
						case 0:
							newTri.C = {(float)(x+1.5),(float)(y+1),(float)(z+1)};
							break;
						case 1:
							newTri.C = {(float)(x+2),(float)(y+1),(float)(z+1.5)};
							break;
						case 2:
							newTri.C = {(float)(x+1.5),(float)(y+1),(float)(z+2)};
							break;
						case 3:
							newTri.C = {(float)(x+1),(float)(y+1),(float)(z+1.5)};
							break;
						case 4:
							newTri.C = {(float)(x+1.5),(float)(y+2),(float)(z+1)};
							break;
						case 7:
							newTri.C = {(float)(x+1),(float)(y+2),(float)(z+1.5)};
							break;
						case 8:
							newTri.C = {(float)(x+1),(float)(y+1.5),(float)(z+1)};
							break;
						case 9:
							newTri.C = {(float)(x+2),(float)(y+1.5),(float)(z+1)};
							break;
						case 11:
							newTri.C = {(float)(x+1),(float)(y+1.5),(float)(z+2)};
							break;
					}

					tempTris.push_back(newTri);
				}
			}
		}
	}

	int l = uniquePoints.size();
	printf("%i unique points\n", l);
	for(int i = 0; i < l; i++) {
		printf("[%.1f,%.1f,%.1f]\n", uniquePoints[i].x, uniquePoints[i].y, uniquePoints[i].z);
	}

	printf("\n%i triangles\n\n\n\n", tempTris.size());
	// Iterate through cubes, adding unique points and triangles to vectors
	// For each point in vector:
		// Add adjusted value (not half of edge) to new array
		// For each triangle with that point in vector:
			// Add normal to sum for average
			// Update tri array with reference to point in point array
		// Update normal array with average normal
	// return mesh created with point, tri, and normal arrays

	return NULL;
}

Point** MarchingCubes::getPoint(Point* points[], int i, int j, int k, int dimension) {
    return &(points[((i*dimension) + j)*dimension + k]);
}