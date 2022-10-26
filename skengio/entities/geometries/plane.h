#pragma once

#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include "mesh.h"

namespace SKEngio {
    class Plane : public Mesh
    {
    public:
        // getters/setters
        void Generate(float width, float height, unsigned int widthSegments, unsigned int heightSegments);

    protected:

    private:
    };
}

#endif