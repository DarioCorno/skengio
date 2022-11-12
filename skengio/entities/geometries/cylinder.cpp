#include "cylinder.h"

#include "glm/glm.hpp"
#include <list>

namespace SKEngio {
	void Cylinder::Generate(float radiusTop, float radiusBottom, float height, int radialSegments, int heightSegments, bool openEnded, float thetaStart, float thetaLength) {
		generateTorso(radiusTop, radiusBottom, height, radialSegments, heightSegments, openEnded, thetaStart, thetaLength);

		//if (openEnded == = false) {
		//	if (radiusTop > 0) generateCap(true);
		//	if (radiusBottom > 0) generateCap(false);
		//}
	}

	void Cylinder::generateTorso(float radiusTop, float radiusBottom, float height, const int radialSegments, const int heightSegments, bool openEnded, float thetaStart, float thetaLength) {

		// this will be used to calculate the normal
		float slope = (radiusBottom - radiusTop) / height;

		// generate vertices, normals and uvs
		int index = 0;
		std::vector< std::vector<int> > indexArray;

		for (int y = 0; y <= heightSegments; y++) {

			std::vector<int> indexRow;

			float v = y / heightSegments;

			// calculate the radius of the current row

			float radius = v * (radiusBottom - radiusTop) + radiusTop;

			for (int x = 0; x <= radialSegments; x++) {

				float u = x / radialSegments;

				float theta = u * thetaLength + thetaStart;

				float sinTheta = sin(theta);
				float cosTheta = cos(theta);

				// vertex

				float vx = radius * sinTheta;
				float vy = -v * height + (height / 2.0f);
				float vz = radius * cosTheta;
				//vertices.push(vertex.x, vertex.y, vertex.z);
				addVertex(vx, vy, vz);

				// normal

				glm::vec3 normal = glm::normalize( glm::vec3(sinTheta, slope, cosTheta) );
				//normals.push(normal.x, normal.y, normal.z);
				addNormal(normal.x, normal.y, normal.z);

				// uv

				addTexCoord(u, 1 - v);

				// save index of vertex in respective row

				indexRow.push_back(index++);

			}

			// now save vertices of the row in our index array
			indexArray.push_back(indexRow);

		}

		// generate indices

		for (int x = 0; x < radialSegments; x++) {

			for (int y = 0; y < heightSegments; y++) {

				// we use the index array to access the correct indices

				int a = indexArray[y][x];
				int b = indexArray[y + 1][x];
				int c = indexArray[y + 1][x + 1];
				int d = indexArray[y][x + 1];

				// faces

				//indices.push(a, b, d);
				//indices.push(b, c, d);
				addTriIndices(a, b, d);
				addTriIndices(b, c, d);

			}

		}

	}
}