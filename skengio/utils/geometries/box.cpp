#include <iostream>
#include <iomanip>
#include <cmath>
#include "box.h"

#include "glm/glm.hpp"

#include "../../defines.h"

#define _X_ 0
#define _Y_ 1
#define _Z_ 2

namespace SKEngio {

	Box::Box() {

	}

	Box::~Box() {

	}

	void Box::Generate(float width, float height, float depth, unsigned int widthSegments, unsigned int heightSegments, unsigned int depthSegments) {
		numberOfVertices = 0;

		_buildPlane(_Z_, _Y_, _X_, -1, -1, depth, height, width, depthSegments, heightSegments); // px
		_buildPlane(_Z_, _Y_, _X_, 1, -1, depth, height, -width, depthSegments, heightSegments); // nx
		_buildPlane(_X_, _Z_, _Y_, 1, 1, width, depth, height, widthSegments, depthSegments); // py
		_buildPlane(_X_, _Z_, _Y_, 1, -1, width, depth, -height, widthSegments, depthSegments); // ny
		_buildPlane(_X_, _Y_, _Z_, 1, -1, width, height, depth, widthSegments, heightSegments); // pz
		_buildPlane(_X_, _Y_, _Z_, -1, -1, width, height, -depth, widthSegments, heightSegments); // nz
	}

	void Box::_buildPlane(unsigned int u, unsigned int v, unsigned int w, int udir, int vdir, float width, float height, float depth, unsigned int gridX, unsigned int gridY) {

		float segmentWidth = (float)width / (float)gridX;
		float segmentHeight = (float)height / (float)gridY;

		float widthHalf = (float)width / 2.0f;
		float heightHalf = (float)height / 2.0f;
		float depthHalf = (float)depth / 2.0f;

		unsigned int gridX1 = gridX + 1;
		unsigned int gridY1 = gridY + 1;

		unsigned int vertexCounter = 0;

		glm::vec3 vector{ 0.0f, 0.0f , 0.0f };

		// generate vertices, normals and uvs

		for (int iy = 0; iy < gridY1; iy++) {

			float y = iy * (float)segmentHeight - heightHalf;

			for (int ix = 0; ix < gridX1; ix++) {

				float x = ix * (float)segmentWidth - widthHalf;

				// set values to correct vector component

				vector[u] = x * (float)udir;
				vector[v] = y * (float)vdir;
				vector[w] = depthHalf;

				// now apply vector to vertex buffer
				addVertex(vector.x, vector.y, vector.z);
				addColor(1.0f, 1.0f, 1.0f, 1.0f);

				// set values to correct vector component
				vector[u] = 0.0f;
				vector[v] = 0.0f;
				vector[w] = (depth > 0.0f) ? 1.0f : -1.0f;

				// now apply vector to normal buffer
				addNormal(vector.x, vector.y, vector.z);

				// uvs
				addTexCoord(ix / (float)gridX, 1.0f - (iy / (float)gridY));

				// counters

				vertexCounter += 1;

			}

		}

		// indices

		// 1. you need three indices to draw a single face
		// 2. a single segment consists of two faces
		// 3. so we need to generate six (2*3) indices per segment

		for (int iy = 0; iy < gridY; iy++) {

			for (int ix = 0; ix < gridX; ix++) {

				unsigned int a = numberOfVertices + ix + gridX1 * iy;
				unsigned int b = numberOfVertices + ix + gridX1 * (iy + 1);
				unsigned int c = numberOfVertices + (ix + 1) + gridX1 * (iy + 1);
				unsigned int d = numberOfVertices + (ix + 1) + gridX1 * iy;

				// faces
				addTriIndices(a, b, d);
				addTriIndices(b, c, d);

			}

		}

		// update total number of vertices
		numberOfVertices += vertexCounter;

	}

}