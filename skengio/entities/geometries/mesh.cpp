#include "mesh.h"

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace SKEngio {

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::clear() {

        vertexes.clear();
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
        return sizeof(float) * 8;
    }

    void Mesh::buildInterleavedArray() {
        std::size_t count = vertexes.size();
        unsigned int ni = 0;
        unsigned int ti = 0;
        for (unsigned int vi = 0; vi < count; vi += 3)
        {
            interleavedVertices.push_back(vertexes[vi]);
            interleavedVertices.push_back(vertexes[vi + 1]);
            interleavedVertices.push_back(vertexes[vi + 2]);

            interleavedVertices.push_back(normals[ni]);
            interleavedVertices.push_back(normals[ni + 1]);
            interleavedVertices.push_back(normals[ni + 2]);

            interleavedVertices.push_back(texCoords[ti]);
            interleavedVertices.push_back(texCoords[ti + 1]);

            ni += 3;
            ti += 2;
        }
    }

    void Mesh::createGLBuffers() {
        if (interleaved) {
            createInterleavedGLBuffers();
        }
        else {
            createBatchGLBuffers();
        }
    }

    void Mesh::createInterleavedGLBuffers() {

        buildInterleavedArray();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        unsigned int vertexCount = interleavedVertices.size();
        //type of buffer, size in bytes of the whole buffer, buffer pointer, draw type
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, interleavedVertices.data(), GL_STATIC_DRAW);

        unsigned int stride = getInterleavedStride();

        //set the vertexes attributes data (let OLG know where the vertex data is)
        // position attribute index 0 in shader, 3 floats, , , how many bytes to jump for the next element, where the value starts in the current element
        //position in shader = 0, made of 3 elements, type float, no normalize, 12 floats to next block, starts at 0 of the block
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)nullptr);
        glEnableVertexAttribArray(0);

        // normal attribute same as above, but starts 3 float from the beginning of the block (3 float for position)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture attribute same as above, but starts 6 float from the beginning of the block (3 float for position,3 for normals)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    void Mesh::draw() {
        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)nullptr);
    }

    void Mesh::createBatchGLBuffers() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        unsigned int bufferSize = sizeof(float) * (vertexes.size() + normals.size() + texCoords.size());
        //generate an empty buffer
        glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);

        //fill the batch buffer
        //byte start, byte size, data*
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexes.size(), vertexes.data());
        unsigned int normalsStart = sizeof(float) * vertexes.size();
        unsigned int normalsSize = sizeof(float) * normals.size();
        glBufferSubData(GL_ARRAY_BUFFER, normalsStart, normalsSize, normals.data());
        unsigned int texStart = (sizeof(float) * vertexes.size()) + (sizeof(float) * normals.size());
        unsigned int texSize = sizeof(float) * texCoords.size();
        glBufferSubData(GL_ARRAY_BUFFER, texStart, texSize, texCoords.data());

        //set the attribs pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(vertexes.size() * sizeof(float) ) );
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)((vertexes.size() + normals.size()) * sizeof(float)) );
        glEnableVertexAttribArray(2);


        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    void Mesh::updateBatchBufferData() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //update the buffer with modified data
        //byte start, byte size, data*
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexes.size(), vertexes.data());
        unsigned int normalsStart = sizeof(float) * vertexes.size();
        unsigned int normalsSize = sizeof(float) * normals.size();
        glBufferSubData(GL_ARRAY_BUFFER, normalsStart, normalsSize, normals.data());
        unsigned int texStart = (sizeof(float) * vertexes.size()) + (sizeof(float) * normals.size());
        unsigned int texSize = sizeof(float) * texCoords.size();
        glBufferSubData(GL_ARRAY_BUFFER, texStart, texSize, texCoords.data());
    }

}