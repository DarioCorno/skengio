#pragma once

#ifndef _SK_MESH_
#define _SK_MESH_

#include <vector>

namespace SKEngio {

    class Mesh {
        public:

        bool useColor = true;

        std::vector<float> vertexes;
        std::vector<float> colors;
        std::vector<float> normals;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;    

        //vertex buffer object and vertex attribs object
        unsigned int VBO, VAO, EBO;
   

        // interleaved
        std::vector<float> interleavedVertices;
        // vertex      color           normal         texcoord
        // xf, yf, zf, rf, gf, bf, af, nxf, nyf, nzf, uf, vf

        int interleavedStride = sizeof(float) * 12;                  // # of bytes to hop to the next vertex (should be 48 bytes)        
        //without color component
        //int interleavedStride = sizeof(float) * 8;                  // # of bytes to hop to the next vertex (should be 32 bytes)        

        Mesh(bool _useColor);

        ~Mesh();

        void clear();

        void addVertex(float x, float y, float z);
        void addColor(float r, float g, float b, float a);
        void addNormal(float nx, float ny, float nz);
        void addTexCoord(float u, float v);

        void addTriIndices(unsigned int i1, unsigned int i2, unsigned int i3);


        unsigned int getVertexCount() const     { return (unsigned int)vertexes.size() / 3; }
        unsigned int getColorsCount() const     { return (unsigned int)colors.size() / 3; }
        unsigned int getNormalCount() const     { return (unsigned int)normals.size() / 3; }
        unsigned int getTexCoordCount() const   { return (unsigned int)texCoords.size() / 2; }
        unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
        unsigned int getTriangleCount() const   { return getIndexCount() / 3; }
        unsigned int getVertexSize() const      { return (unsigned int)vertexes.size() * sizeof(float); }
        unsigned int getNormalSize() const      { return (unsigned int)normals.size() * sizeof(float); }
        unsigned int getTexCoordSize() const    { return (unsigned int)texCoords.size() * sizeof(float); }
        unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }

        const float* getVerticesPt() const        { return vertexes.data(); }
        const float* getColorsPt() const         { return colors.data(); }
        const float* getNormalsPt() const         { return normals.data(); }
        const float* getTexCoordsPt() const       { return texCoords.data(); }
        const unsigned int* getIndicesPt() const  { return indices.data(); }        

        unsigned int getInterleavedStride();

        void buildInterleavedArray();

        void createGLBuffers();

        void draw();

    };
}




#endif