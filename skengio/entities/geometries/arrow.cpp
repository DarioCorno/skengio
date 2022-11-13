#include "arrow.h"

namespace SKEngio {
	void Arrow::GenerateZ() {

		//body quad
		addVertex(-0.25, 0.0f, 0.0f);	//A
		addVertex(0.25, 0.0f, 0.0f);		//B
		addVertex(0.25, 0.0f, -2.0f);	//C
		addVertex(-0.25, 0.0f, -2.0f);	//D

		addNormal(0.0f, 0.0f, 1.0f);
		addNormal(0.0f, 0.0f, 1.0f);
		addNormal(0.0f, 0.0f, 1.0f);
		addNormal(0.0f, 0.0f, 1.0f);

		addTexCoord(0.0f, 0.0f);
		addTexCoord(0.5f, 0.0f);
		addTexCoord(0.5f, 0.5f);

		addTriIndices(0, 1, 2);
		addTriIndices(0, 2, 3);

		//arrow triangle point
		addVertex(-0.5f, 0.0f, -2.0f);
		addVertex(0.5f, 0.0f, -2.0f);
		addVertex(0.0f, 0.0f, -3.0f);

		addNormal(0.0f, 0.0f, 1.0f);
		addNormal(0.0f, 0.0f, 1.0f);
		addNormal(0.0f, 0.0f, 1.0f);

		addTexCoord(0.0f, 0.5f);
		addTexCoord(0.5f, 0.5f);
		addTexCoord(0.5f, 1.0f);

		addTriIndices(4, 5, 6);

		createGLBuffers();
	}
}
