#include "vertexobject.h"

using namespace IceFairy;

_VertexObject::_VertexObject()
    : ibo(0),
      vbo(0),
      numIndices(0)
{ }

_VertexObject::_VertexObject(
    unsigned int* indices,
    unsigned int numIndices,
    Vector3f* points,
    Vector3f* normals,
    unsigned int numPoints)
    : _VertexObject()
{
    BuildIndices(indices, numIndices);

    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < numPoints; i++) {
        vertices.push_back(Vertex(points[i], normals[i]));
    }

    BuildVertices(vertices);
}

_VertexObject::_VertexObject(
    unsigned int* indices,
    unsigned int numIndices,
    Vector3f* points,
    Vector3f* normals,
    Vector2f* texcoords,
    unsigned int numPoints)
    : _VertexObject()
{
    BuildIndices(indices, numIndices);

    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < numPoints; i++) {
		vertices.push_back(Vertex(points[i], normals[i], texcoords[i]));
    }

    BuildVertices(vertices);
}

_VertexObject::_VertexObject(
    const std::vector<unsigned int>& indices,
    const std::vector<Vector3f>& points,
    const std::vector<Vector3f>& normals)
    : _VertexObject()
{
    if (points.size() != normals.size())
        throw InvalidPointSpecificationException();

    BuildIndices(indices);

    std::vector<Vertex> vertices;

    for (unsigned int i = 0; i < points.size(); i++) {
		vertices.push_back(Vertex(points[i], normals[i]));
    }

    BuildVertices(vertices);
}

_VertexObject::~_VertexObject() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
}

void _VertexObject::BuildIndices(const std::vector<unsigned int>& indices) {
	glDeleteBuffers(1, &ibo);

    numIndices = indices.size();

	if (numIndices != 0) {
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
	}
}

void _VertexObject::BuildVertices(const std::vector<Vertex>& vertices) {
	glDeleteBuffers(1, &vbo);

	if (!vertices.empty()) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	}
}

void _VertexObject::BuildIndices(unsigned int* indices, unsigned int numIndices) {
	glDeleteBuffers(1, &ibo);

    this->numIndices = numIndices;

	if (numIndices != 0) {
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);
	}
}

void _VertexObject::Draw(void) {
    glEnableVertexAttribArray(ICE_FAIRY_ATTRIB_POSITION);
    glEnableVertexAttribArray(ICE_FAIRY_ATTRIB_NORMAL);
    glEnableVertexAttribArray(ICE_FAIRY_ATTRIB_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(ICE_FAIRY_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
    glVertexAttribPointer(ICE_FAIRY_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 12);
    glVertexAttribPointer(ICE_FAIRY_ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 24);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(ICE_FAIRY_ATTRIB_POSITION);
    glDisableVertexAttribArray(ICE_FAIRY_ATTRIB_NORMAL);
    glDisableVertexAttribArray(ICE_FAIRY_ATTRIB_TEXCOORD);
}