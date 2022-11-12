#pragma once

#ifndef GEOMETRY_CYLINDER_H
#define GEOMETRY_CYLINDER_H

#include "mesh.h"

#define PI acos(-1)

namespace SKEngio {
    class Cylinder : public Mesh
    {
    public:
        // getters/setters
        void Generate(float radiusTop = 1.0f, float radiusBottom = 1.0f, float height = 1.0f, int radialSegments = 8, int heightSegments = 1, bool openEnded = false, float thetaStart = 0.0f, float thetaLength = PI * 2);

    protected:

    private:
        void generateTorso(float radiusTop, float radiusBottom, float height, const int radialSegments, const int heightSegments, bool openEnded, float thetaStart, float thetaLength);

    };
}

#endif