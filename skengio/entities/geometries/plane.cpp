#include "plane.h"

namespace SKEngio {
	void Plane::Generate(float width, float height, unsigned int widthSegments, unsigned int heightSegments) {
		float width_half = width / 2.0f;
		float height_half = height / 2.0f;

		unsigned int gridX = widthSegments;
		unsigned int gridY = heightSegments;

		unsigned int gridX1 = gridX + 1;
		unsigned int gridY1 = gridY + 1;

		float segment_width = width / (float)gridX;
		float segment_height = height / (float)gridY;

		for (unsigned int iy = 0; iy < gridY1; iy++) {

			float y = iy * segment_height - height_half;

			for (unsigned int ix = 0; ix < gridX1; ix++) {

				float x = ix * segment_width - width_half;

				addVertex(x, -y, 0.0f);
				addNormal(0.0f, 0.0f, 1.0f);

				addTexCoord(ix / (float)gridX, 1.0f - (iy / (float)gridY) );
			}

		}

		for (unsigned int iy = 0; iy < gridY; iy++) {

			for (unsigned int ix = 0; ix < gridX; ix++) {

				unsigned int a = ix + gridX1 * iy;
				unsigned int b = ix + gridX1 * (iy + 1);
				unsigned int c = (ix + 1) + gridX1 * (iy + 1);
				unsigned int d = (ix + 1) + gridX1 * iy;

				addTriIndices(a, b, d);
				addTriIndices(b, c, d);

			}

		}

		createGLBuffers();
	}
}
