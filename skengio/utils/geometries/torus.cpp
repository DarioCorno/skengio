
#include <cmath>
#include "torus.h"
#include "../../defines.h"
#include "glm/glm.hpp"

namespace SKEngio {
    void Torus::Generate(float tubeR, float mainR, int mainSteps, int tubeSteps, float radArc = M_PI * 2.0f) {
        float arc = radArc;

        for (int j = 0; j <= mainSteps; j++) {

            for (int i = 0; i <= tubeSteps; i++) {

                float u = i / (float)tubeSteps * arc;
                float v = j / (float)mainSteps * M_PI * 2.0f;

                // vertex

                float x = (mainR + tubeR * cos(v)) * cos(u);
                float y = (mainR + tubeR * cos(v)) * sin(u);
                float z = tubeR * sin(v);
                glm::vec3 vertex(x, y, z);
                addVertex(x, y, z);
                addColor(1.0f, 1.0f, 1.0f, 1.0f);

                // normal
                glm::vec3 center(mainR * cos(u), mainR * sin(u), 0.0f);
                glm::vec3 normal = glm::normalize(vertex - center);

                addNormal(normal.x, normal.y, normal.z);

                // uv
                addTexCoord(i / (float)tubeSteps, j / (float)mainSteps);

            }

        }

        for (int j = 1; j <= mainSteps; j++) {

            for (int i = 1; i <= tubeSteps; i++) {

                // indices

                unsigned int a = (tubeSteps + 1) * j + i - 1;
                unsigned int b = (tubeSteps + 1) * (j - 1) + i - 1;
                unsigned int c = (tubeSteps + 1) * (j - 1) + i;
                unsigned int d = (tubeSteps + 1) * j + i;

                // faces

                addTriIndices(a, b, d);
                addTriIndices(b, c, d);

            }

        }
    }

}