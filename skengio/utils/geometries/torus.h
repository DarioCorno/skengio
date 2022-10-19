#pragma once

#ifndef GEOMETRY_TORUS_H
#define GEOMETRY_TORUS_H

#include <skengio/core.h>

#include <vector>
#include <map>

namespace SKEngio {

    class Mesh;

    class Torus : public Mesh
    {
    public:
        // ctor/dtor
        Torus();
        ~Torus();

        // getters/setters
        void Generate(float tubeR, float mainR, int mainSteps, int tubeSteps, float radArc);

    protected:

    private:

    };

}

#endif
