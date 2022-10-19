#pragma once

#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include <vector>
#include <map>

#include "mesh.h"

namespace SKEngio {
    class Plane : public Mesh
    {
    public:
        // ctor/dtor
        Plane();
        ~Plane();

        // getters/setters
        void Generate(float width, float height, unsigned int widthSegments, unsigned int heightSegments);

    protected:

    private:
    };
}

#endif