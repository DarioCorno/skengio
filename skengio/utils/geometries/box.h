#pragma once

#ifndef GEOMETRY_BOX_H
#define GEOMETRY_BOX_H

#include <vector>
#include <map>

#include "mesh.h"

namespace SKEngio {
    class Box : public Mesh
    {
    public:
        // ctor/dtor
        Box();
        ~Box();

        // getters/setters
        void Generate(float width, float height, float depth, unsigned int widthSegments, unsigned int heightSegments, unsigned int depthSegments);

    protected:

    private:
        void _buildPlane(unsigned int u, unsigned int v, unsigned int w, int udir, int vdir, float width, float height, float depth, unsigned int gridX, unsigned int gridY);
        unsigned int numberOfVertices;
    };
}

#endif