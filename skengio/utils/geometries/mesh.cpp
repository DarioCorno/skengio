#include "mesh.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace SKEngio {

    Mesh::Mesh(bool _useColor) {
        useColor = _useColor;
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Mesh::clear() {

        vertexes.clear();
        colors.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();
        interleavedVertices.clear();

    }

    void Mesh::addVertex(float x, float y, float z) {
        vertexes.push_back(x);
        vertexes.push_back(y);
        vertexes.push_back(z);
    }

    void Mesh::addColor(float r, float g, float b, float a) {
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
        colors.push_back(a);
    }

    void Mesh::addNormal(float nx, float ny, float nz) {
        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);
    }

    void Mesh::addTexCoord(float u, float v) {
        texCoords.push_back(u);
        texCoords.push_back(v);
    }

    void Mesh::addTriIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    unsigned int Mesh::getInterleavedStride() {
        return sizeof(float) * 12;
    }

    void Mesh::buildInterleavedArray() {
        std::size_t count = vertexes.size();
        int vi = 0;
        int ci = 0;
        int ni = 0;
        int ti = 0;
        for(vi = 0; vi < count; vi += 3)
        {
            interleavedVertices.push_back(vertexes[vi]);
            interleavedVertices.push_back(vertexes[vi+1]);
            interleavedVertices.push_back(vertexes[vi+2]);

            interleavedVertices.push_back(colors[ci]);
            interleavedVertices.push_back(colors[ci+1]);
            interleavedVertices.push_back(colors[ci+2]);
            interleavedVertices.push_back(colors[ci+3]);

            interleavedVertices.push_back(normals[ni]);
            interleavedVertices.push_back(normals[ni+1]);
            interleavedVertices.push_back(normals[ni+2]);

            interleavedVertices.push_back(texCoords[ti]);
            interleavedVertices.push_back(texCoords[ti+1]);

            ni += 3;
            ci += 4;
            ti += 2;
        }        
    }

    void Mesh::createGLBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);       
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //type of buffer, size in bytes of the whole buffer, buffer pointer, draw type
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * interleavedVertices.size(), interleavedVertices.data() , GL_STATIC_DRAW);        

        unsigned int stride = getInterleavedStride();

        //set the vertexes attributes data (let OLG know where the vertex data is)
        // position attribute index 0 in shader, 3 floats, , , how many bytes to jump for the next element, where the value starts in the current element
        //position in shader = 0, made of 3 elements, type float, no normalize, 12 floats to next block, starts at 0 of the block
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute (same as above, but starts 3 float from the beginning of the block)
        //position in shader = 0, made of 4 elements, type float, no normalize, 12 floats to next block, starts at 3 of the block
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // normal attribute same as above, but starts 7 float from the beginning of the block (3 float for position, 4 float for color)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // texture attribute same as above, but starts 7 float from the beginning of the block (3 float for position, 4 float for color, 3 for normals)
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(10 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);        
    }

    void Mesh::draw() {
        glBindVertexArray(VAO);       
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0 );
    }

}