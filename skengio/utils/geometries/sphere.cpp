///////////////////////////////////////////////////////////////////////////////
// Sphere.cpp
// ==========
// Sphere for OpenGL with (radius, sectors, stacks)
// The min number of sectors is 3 and the min number of stacks are 2.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2017-11-01
// UPDATED: 2020-05-20
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <iomanip>
#include <cmath>
#include "sphere.h"

namespace SKEngio {

    // constants //////////////////////////////////////////////////////////////////
    const int MIN_SECTOR_COUNT = 3;
    const int MIN_STACK_COUNT  = 2;



    ///////////////////////////////////////////////////////////////////////////////
    // ctor
    ///////////////////////////////////////////////////////////////////////////////
    Sphere::Sphere()
    {
    }



    ///////////////////////////////////////////////////////////////////////////////
    // setters
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::Generate(float _radius, int _subdivision)
    {
        radius = _radius;
        subdivision = _subdivision;

        buildVerticesSmooth();
    }

    std::vector<float> Sphere::computeIcosahedronVertices() {
        const float PI = acos(-1);
        const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

        std::vector<float> vertices(12 * 3);    // 12 vertices
        int i1, i2;                             // indices
        float z, xy;                            // coords
        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 2nd row
        float hAngle2 = -PI / 2;                // start from -90 deg at 3rd row

        // the first top vertex (0, 0, r)
        vertices[0] = 0;
        vertices[1] = 0;
        vertices[2] = radius;

        // 10 vertices at 2nd and 3rd rows
        for(int i = 1; i <= 5; ++i)
        {
            i1 = i * 3;         // for 2nd row
            i2 = (i + 5) * 3;   // for 3rd row

            z = radius * sinf(V_ANGLE);             // elevaton
            xy = radius * cosf(V_ANGLE);

            vertices[i1] = xy * cosf(hAngle1);      // x
            vertices[i2] = xy * cosf(hAngle2);
            vertices[i1 + 1] = xy * sinf(hAngle1);  // x
            vertices[i2 + 1] = xy * sinf(hAngle2);
            vertices[i1 + 2] = z;                   // z
            vertices[i2 + 2] = -z;

            // next horizontal angles
            hAngle1 += H_ANGLE;
            hAngle2 += H_ANGLE;
        }

        // the last bottom vertex (0, 0, -r)
        i1 = 11 * 3;
        vertices[i1] = 0;
        vertices[i1 + 1] = 0;
        vertices[i1 + 2] = -radius;

        return vertices;        
    }


    ///////////////////////////////////////////////////////////////////////////////
    // add a subdivided vertex attribs (vertex, normal, texCoord) to arrays, then
    // return its index value
    // If it is a shared vertex, remember its index, so it can be re-used
    ///////////////////////////////////////////////////////////////////////////////
    unsigned int Sphere::addSubVertexAttribs(const float v[3], const float n[3], const float t[2])
    {
        unsigned int index;     // return value;

        // check if is shared vertex or not first
        if(isSharedTexCoord(t))
        {
            // find if it does already exist in sharedIndices map using (s,t) key
            // if not in the list, add the vertex attribs to arrays and return its index
            // if exists, return the current index
            std::pair<float, float> key = std::make_pair(t[0], t[1]);
            std::map<std::pair<float, float>, unsigned int>::iterator iter = sharedIndices.find(key);
            if(iter == sharedIndices.end())
            {
                addVertex(v[0], v[1], v[2]);
                addColor(1.0f, 0.0f, 1.0f, 0.2f);
                addNormal(n[0], n[1], n[2]);
                addTexCoord(t[0], t[1]);
                index = texCoords.size() / 2 - 1;
                sharedIndices[key] = index;
            }
            else
            {
                index = iter->second;
            }
        }
        // not shared
        else
        {
            addVertex(v[0], v[1], v[2]);
            addColor(1.0f, 0.0f, 1.0f, 0.2f);
            addNormal(n[0], n[1], n[2]);
            addTexCoord(t[0], t[1]);
            index = texCoords.size() / 2 - 1;
        }

        return index;
    }    


    ///////////////////////////////////////////////////////////////////////////////
    // This function used 20 non-shared line segments to determine if the given
    // texture coordinate is shared or no. If it is on the line segments, it is also
    // non-shared point
    //   00  01  02  03  04         //
    //   /\  /\  /\  /\  /\         //
    //  /  \/  \/  \/  \/  \        //
    // 05  06  07  08  09   \       //
    //   \   10  11  12  13  14     //
    //    \  /\  /\  /\  /\  /      //
    //     \/  \/  \/  \/  \/       //
    //      15  16  17  18  19      //
    ///////////////////////////////////////////////////////////////////////////////
    bool Sphere::isSharedTexCoord(const float t[2])
    {
        // 20 non-shared line segments
        const float S = 1.0f / 11;  // texture steps
        const float T = 1.0f / 3;
        static float segments[] = { S, 0,       0, T,       // 00 - 05
                                    S, 0,       S*2, T,     // 00 - 06
                                    S*3, 0,     S*2, T,     // 01 - 06
                                    S*3, 0,     S*4, T,     // 01 - 07
                                    S*5, 0,     S*4, T,     // 02 - 07
                                    S*5, 0,     S*6, T,     // 02 - 08
                                    S*7, 0,     S*6, T,     // 03 - 08
                                    S*7, 0,     S*8, T,     // 03 - 09
                                    S*9, 0,     S*8, T,     // 04 - 09
                                    S*9, 0,     1, T*2,     // 04 - 14
                                    0, T,       S*2, 1,     // 05 - 15
                                    S*3, T*2,   S*2, 1,     // 10 - 15
                                    S*3, T*2,   S*4, 1,     // 10 - 16
                                    S*5, T*2,   S*4, 1,     // 11 - 16
                                    S*5, T*2,   S*6, 1,     // 11 - 17
                                    S*7, T*2,   S*6, 1,     // 12 - 17
                                    S*7, T*2,   S*8, 1,     // 12 - 18
                                    S*9, T*2,   S*8, 1,     // 13 - 18
                                    S*9, T*2,   S*10, 1,    // 13 - 19
                                    1, T*2,     S*10, 1 };  // 14 - 19

        // check the point with all 20 line segments
        // if it is on the line segment, it is non-shared
        int count = (int)(sizeof(segments) / sizeof(segments[0]));
        for(int i = 0, j = 2; i < count; i+=4, j+=4)
        {
            if(isOnLineSegment(&segments[i], &segments[j], t))
                return false;   // not shared
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // determine a point c is on the line segment a-b
    ///////////////////////////////////////////////////////////////////////////////
    bool Sphere::isOnLineSegment(const float a[2], const float b[2], const float c[2])
    {
        const float EPSILON = 0.0001f;

        // cross product must be 0 if c is on the line
        float cross = ((b[0] - a[0]) * (c[1] - a[1])) - ((b[1] - a[1]) * (c[0] - a[0]));
        if(cross > EPSILON || cross < -EPSILON)
            return false;

        // c must be within a-b
        if((c[0] > a[0] && c[0] > b[0]) || (c[0] < a[0] && c[0] < b[0]))
            return false;
        if((c[1] > a[1] && c[1] > b[1]) || (c[1] < a[1] && c[1] < b[1]))
            return false;

        return true;    // all passed, it is on the line segment
    }


    ///////////////////////////////////////////////////////////////////////////////
    // build vertices of sphere with smooth shading using parametric equation
    // x = r * cos(u) * cos(v)
    // y = r * cos(u) * sin(v)
    // z = r * sin(u)
    // where u: stack(latitude) angle (-90 <= u <= 90)
    //       v: sector(longitude) angle (0 <= v <= 360)
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::buildVerticesSmooth()
    {
        //const float S_STEP = 1 / 11.0f;         // horizontal texture step
        //const float T_STEP = 1 / 3.0f;          // vertical texture step
        const float S_STEP = 186 / 2048.0f;     // horizontal texture step
        const float T_STEP = 322 / 1024.0f;     // vertical texture step

        // compute 12 vertices of icosahedron
        // NOTE: v0 (top), v11(bottom), v1, v6(first vert on each row) cannot be
        // shared for smooth shading (they have different texcoords)
        std::vector<float> tmpVertices = computeIcosahedronVertices();

        // clear memory of prev arrays
        std::vector<float>().swap(vertexes);
        std::vector<float>().swap(normals);
        std::vector<float>().swap(texCoords);
        std::vector<unsigned int>().swap(indices);
        std::map<std::pair<float, float>, unsigned int>().swap(sharedIndices);

        float v[3];                             // vertex
        float n[3];                             // normal
        float scale;                            // scale factor for normalization

        // smooth icosahedron has 14 non-shared (0 to 13) and
        // 8 shared vertices (14 to 21) (total 22 vertices)
        //  00  01  02  03  04          //
        //  /\  /\  /\  /\  /\          //
        // /  \/  \/  \/  \/  \         //
        //10--14--15--16--17--11        //
        // \  /\  /\  /\  /\  /\        //
        //  \/  \/  \/  \/  \/  \       //
        //  12--18--19--20--21--13      //
        //   \  /\  /\  /\  /\  /       //
        //    \/  \/  \/  \/  \/        //
        //    05  06  07  08  09        //
        // add 14 non-shared vertices first (index from 0 to 13)
        addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v0 (top)
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, 1);
        addTexCoord(S_STEP, 0);

        addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v1
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, 1);
        addTexCoord(S_STEP * 3, 0);

        addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v2
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, 1);
        addTexCoord(S_STEP * 5, 0);

        addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v3
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, 1);
        addTexCoord(S_STEP * 7, 0);

        addVertex(tmpVertices[0], tmpVertices[1], tmpVertices[2]);      // v4
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, 1);
        addTexCoord(S_STEP * 9, 0);

        addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v5 (bottom)
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, -1);
        addTexCoord(S_STEP * 2, T_STEP * 3);

        addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v6
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, -1);
        addTexCoord(S_STEP * 4, T_STEP * 3);

        addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v7
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, -1);
        addTexCoord(S_STEP * 6, T_STEP * 3);

        addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v8
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, -1);
        addTexCoord(S_STEP * 8, T_STEP * 3);

        addVertex(tmpVertices[33], tmpVertices[34], tmpVertices[35]);   // v9
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(0, 0, -1);
        addTexCoord(S_STEP * 10, T_STEP * 3);

        v[0] = tmpVertices[3];  v[1] = tmpVertices[4];  v[2] = tmpVertices[5];  // v10 (left)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(0, T_STEP);

        addVertex(v[0], v[1], v[2]);                                            // v11 (right)
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 10, T_STEP);

        v[0] = tmpVertices[18]; v[1] = tmpVertices[19]; v[2] = tmpVertices[20]; // v12 (left)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP, T_STEP * 2);

        addVertex(v[0], v[1], v[2]);                                            // v13 (right)
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 11, T_STEP * 2);

        // add 8 shared vertices to array (index from 14 to 21)
        v[0] = tmpVertices[6];  v[1] = tmpVertices[7];  v[2] = tmpVertices[8];  // v14 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 2, T_STEP);
        sharedIndices[std::make_pair(S_STEP * 2, T_STEP)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[9];  v[1] = tmpVertices[10]; v[2] = tmpVertices[11]; // v15 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 4, T_STEP);
        sharedIndices[std::make_pair(S_STEP * 4, T_STEP)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[12]; v[1] = tmpVertices[13]; v[2] = tmpVertices[14]; // v16 (shared)
        scale = computeScaleForLength(v, 1);
        n[0] = v[0] * scale;    n[1] = v[1] * scale;    n[2] = v[2] * scale;
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 6, T_STEP);
        sharedIndices[std::make_pair(S_STEP * 6, T_STEP)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[15]; v[1] = tmpVertices[16]; v[2] = tmpVertices[17]; // v17 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 8, T_STEP);
        sharedIndices[std::make_pair(S_STEP * 8, T_STEP)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[21]; v[1] = tmpVertices[22]; v[2] = tmpVertices[23]; // v18 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 3, T_STEP * 2);
        sharedIndices[std::make_pair(S_STEP * 3, T_STEP * 2)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[24]; v[1] = tmpVertices[25]; v[2] = tmpVertices[26]; // v19 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 5, T_STEP * 2);
        sharedIndices[std::make_pair(S_STEP * 5, T_STEP * 2)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[27]; v[1] = tmpVertices[28]; v[2] = tmpVertices[29]; // v20 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 7, T_STEP * 2);
        sharedIndices[std::make_pair(S_STEP * 7, T_STEP * 2)] = texCoords.size() / 2 - 1;

        v[0] = tmpVertices[30]; v[1] = tmpVertices[31]; v[2] = tmpVertices[32]; // v21 (shared)
        computeVertexNormal(v, n);
        addVertex(v[0], v[1], v[2]);
        addColor(1.0f, 0.0f, 1.0f, 0.2f);
        addNormal(n[0], n[1], n[2]);
        addTexCoord(S_STEP * 9, T_STEP * 2);
        sharedIndices[std::make_pair(S_STEP * 9, T_STEP * 2)] = texCoords.size() / 2 - 1;

        // build index list for icosahedron (20 triangles)
        addTriIndices( 0, 10, 14);      // 1st row (5 tris)
        addTriIndices( 1, 14, 15);
        addTriIndices( 2, 15, 16);
        addTriIndices( 3, 16, 17);
        addTriIndices( 4, 17, 11);
        addTriIndices(10, 12, 14);      // 2nd row (10 tris)
        addTriIndices(12, 18, 14);
        addTriIndices(14, 18, 15);
        addTriIndices(18, 19, 15);
        addTriIndices(15, 19, 16);
        addTriIndices(19, 20, 16);
        addTriIndices(16, 20, 17);
        addTriIndices(20, 21, 17);
        addTriIndices(17, 21, 11);
        addTriIndices(21, 13, 11);
        addTriIndices( 5, 18, 12);      // 3rd row (5 tris)
        addTriIndices( 6, 19, 18);
        addTriIndices( 7, 20, 19);
        addTriIndices( 8, 21, 20);
        addTriIndices( 9, 13, 21);

        // subdivide icosahedron
        subdivideVerticesSmooth();

    }

    ///////////////////////////////////////////////////////////////////////////////
    // divide a trianlge (v1-v2-v3) into 4 sub triangles by adding middle vertices
    // (newV1, newV2, newV3) and repeat N times
    // If subdivision=0, do nothing.
    //         v1           //
    //        / \           //
    // newV1 *---* newV3    //
    //      / \ / \         //
    //    v2---*---v3       //
    //        newV2         //
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::subdivideVerticesSmooth()
    {
        std::vector<unsigned int> tmpIndices;
        int indexCount;
        unsigned int i1, i2, i3;            // indices from original triangle
        const float *v1, *v2, *v3;          // ptr to original vertices of a triangle
        const float *t1, *t2, *t3;          // ptr to original texcoords of a triangle
        float newV1[3], newV2[3], newV3[3]; // new subdivided vertex positions
        float newN1[3], newN2[3], newN3[3]; // new subdivided normals
        float newT1[2], newT2[2], newT3[2]; // new subdivided texture coords
        unsigned int newI1, newI2, newI3;   // new subdivided indices
        int i, j;

        // iteration for subdivision
        for(i = 1; i <= subdivision; ++i)
        {
            // copy prev indices
            tmpIndices = indices;

            // clear prev arrays
            indices.clear();

            indexCount = (int)tmpIndices.size();
            for(j = 0; j < indexCount; j += 3)
            {
                // get 3 indices of each triangle
                i1 = tmpIndices[j];
                i2 = tmpIndices[j+1];
                i3 = tmpIndices[j+2];

                // get 3 vertex attribs from prev triangle
                v1 = &vertexes[i1 * 3];
                v2 = &vertexes[i2 * 3];
                v3 = &vertexes[i3 * 3];
                t1 = &texCoords[i1 * 2];
                t2 = &texCoords[i2 * 2];
                t3 = &texCoords[i3 * 2];

                // get 3 new vertex attribs by spliting half on each edge
                computeHalfVertex(v1, v2, radius, newV1);
                computeHalfVertex(v2, v3, radius, newV2);
                computeHalfVertex(v1, v3, radius, newV3);
                computeHalfTexCoord(t1, t2, newT1);
                computeHalfTexCoord(t2, t3, newT2);
                computeHalfTexCoord(t1, t3, newT3);
                computeVertexNormal(newV1, newN1);
                computeVertexNormal(newV2, newN2);
                computeVertexNormal(newV3, newN3);

                // add new vertices/normals/texcoords to arrays
                // It will check if it is shared/non-shared and return index
                newI1 = addSubVertexAttribs(newV1, newN1, newT1);
                newI2 = addSubVertexAttribs(newV2, newN2, newT2);
                newI3 = addSubVertexAttribs(newV3, newN3, newT3);

                // add 4 new triangle indices
                addTriIndices(i1, newI1, newI3);
                addTriIndices(newI1, i2, newI2);
                addTriIndices(newI1, newI2, newI3);
                addTriIndices(newI3, newI2, i3);

            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // find middle point of 2 vertices
    // NOTE: new vertex must be resized, so the length is equal to the given length
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3])
    {
        newV[0] = v1[0] + v2[0];
        newV[1] = v1[1] + v2[1];
        newV[2] = v1[2] + v2[2];
        float scale = computeScaleForLength(newV, length);
        newV[0] *= scale;
        newV[1] *= scale;
        newV[2] *= scale;
    }



    ///////////////////////////////////////////////////////////////////////////////
    // find middle texcoords of 2 tex coords and return new coord (3rd param)
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2])
    {
        newT[0] = (t1[0] + t2[0]) * 0.5f;
        newT[1] = (t1[1] + t2[1]) * 0.5f;
    }



    ///////////////////////////////////////////////////////////////////////////////
    // return vertex normal (2nd param) by mormalizing the vertex vector
    ///////////////////////////////////////////////////////////////////////////////
    void Sphere::computeVertexNormal(const float v[3], float normal[3])
    {
        // normalize
        float scale = computeScaleForLength(v, 1);
        normal[0] = v[0] * scale;
        normal[1] = v[1] * scale;
        normal[2] = v[2] * scale;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // get the scale factor for vector to resize to the given length of vector
    ///////////////////////////////////////////////////////////////////////////////
    float Sphere::computeScaleForLength(const float v[3], float length)
    {
        // and normalize the vector then re-scale to new radius
        return length / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }



}