#pragma once

#ifndef GEOMETRY_TORUS_H
#define GEOMETRY_TORUS_H

#include "mesh.h"

namespace SKEngio {
    class Torus : public Mesh
    {
    public:
        // getters/setters
        void Generate(float tubeR, float mainR, int mainSteps, int tubeSteps, float radArc);

    protected:

    private:

    };

}

#endif
