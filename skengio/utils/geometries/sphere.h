///////////////////////////////////////////////////////////////////////////////
// Sphere.h
// ========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and The min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2020-05-20
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>
#include <map>

#include "mesh.h"

namespace SKEngio {
    class Sphere : public Mesh
    {
    public:
        // ctor/dtor
        Sphere();
        ~Sphere();

        // getters/setters
        float getRadius() const                 { return radius; }
        void Generate(float _radius, int _subdivisions);

    protected:

    private:
        // member functions
        void buildVerticesSmooth();
        std::vector<float>computeIcosahedronVertices();
        unsigned int addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);
        bool isSharedTexCoord(const float t[2]);
        bool isOnLineSegment(const float a[2], const float b[2], const float c[2]);
        void computeVertexNormal(const float v[3], float normal[3]);
        float computeScaleForLength(const float v[3], float length);
        void subdivideVerticesSmooth();
        void computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
        void computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2]);



        // memeber vars
        float radius;
        int subdivision = 2;

        std::map<std::pair<float, float>, unsigned int> sharedIndices;   // indices of shared vertices, key is tex coord (s,t)


    };

}

#endif
