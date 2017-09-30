#include "cubeobject.h"

using namespace IceFairy;

CubeObject::CubeObject(float size) {
    float sz = size / 2.0f;

    unsigned int indices[36] = {
        0,  1,  2,  2,  3,  0,  // Bottom
        4,  5,  6,  6,  7,  4,  // Top
        8,  9,  10, 10, 11, 8,  // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Front
        20, 21, 22, 22, 23, 20  // Back
    };

    IceFairy::Vector3f points[24] = {
        // Bottom
        IceFairy::Vector3f(-sz, -sz, -sz),
        IceFairy::Vector3f(-sz,  sz, -sz),
        IceFairy::Vector3f( sz,  sz, -sz),
        IceFairy::Vector3f( sz, -sz, -sz),
        // Top
        IceFairy::Vector3f( sz, -sz,  sz),
        IceFairy::Vector3f( sz,  sz,  sz),
        IceFairy::Vector3f(-sz,  sz,  sz),
        IceFairy::Vector3f(-sz, -sz,  sz),
        // Left
        IceFairy::Vector3f(-sz, -sz, -sz),
        IceFairy::Vector3f(-sz, -sz,  sz),
        IceFairy::Vector3f(-sz,  sz,  sz),
        IceFairy::Vector3f(-sz,  sz, -sz),
        // Right
        IceFairy::Vector3f( sz,  sz, -sz),
        IceFairy::Vector3f( sz,  sz,  sz),
        IceFairy::Vector3f( sz, -sz,  sz),
        IceFairy::Vector3f( sz, -sz, -sz),
        // Front
        IceFairy::Vector3f( sz, -sz, -sz),
        IceFairy::Vector3f( sz, -sz,  sz),
        IceFairy::Vector3f(-sz, -sz,  sz),
        IceFairy::Vector3f(-sz, -sz, -sz),
        // Back
        IceFairy::Vector3f(-sz,  sz, -sz),
        IceFairy::Vector3f(-sz,  sz,  sz),
        IceFairy::Vector3f( sz,  sz,  sz),
        IceFairy::Vector3f( sz,  sz, -sz),
    };

    IceFairy::Vector3f normals[24] = {
        // Bottom
        IceFairy::Vector3f(0.0f, 0.0f, -1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, -1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, -1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, -1.0f),
        // Top
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        IceFairy::Vector3f(0.0f, 0.0f, 1.0f),
        // Left
        IceFairy::Vector3f(-1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(-1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(-1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(-1.0f, 0.0f, 0.0f),
        // Right
        IceFairy::Vector3f(1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(1.0f, 0.0f, 0.0f),
        IceFairy::Vector3f(1.0f, 0.0f, 0.0f),
        // Front
        IceFairy::Vector3f(0.0f, -1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, -1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, -1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, -1.0f, 0.0f),
        // Back
        IceFairy::Vector3f(0.0f, 1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, 1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, 1.0f, 0.0f),
        IceFairy::Vector3f(0.0f, 1.0f, 0.0f),
    };

    IceFairy::Vector2f texcoords[24] = {
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 0.0f),
        IceFairy::Vector2f(1.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 1.0f),
        IceFairy::Vector2f(0.0f, 0.0f)
    };

    BuildIndices(indices, 36);

    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < 24; i++) {
		vertices.push_back(IceFairy::Vertex(points[i], normals[i], texcoords[i]));
    }

    BuildVertices(vertices);
}