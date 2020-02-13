#include "vertexobject.h"

using namespace IceFairy;

VertexObject::VertexObject(
	const std::vector<unsigned int>& indices,
	const std::vector<Vertex>& vertices):
	indices(indices),
	vertices(vertices)
{ }

VertexObject::~VertexObject() {
}

void VertexObject::CleanUp(vma::Allocator& allocator) {
	allocator.destroyBuffer(indexBuffer, indexBufferMemory);
	allocator.destroyBuffer(vertexBuffer, vertexBufferMemory);
}

void IceFairy::VertexObject::AssignIndexBuffer(std::pair<vk::Buffer, vma::Allocation>& indexBufferAllocation) {
	std::tie(indexBuffer, indexBufferMemory) = indexBufferAllocation;
}

void IceFairy::VertexObject::AssignVertexBuffer(std::pair<vk::Buffer, vma::Allocation>& vertexBufferAllocation) {
	std::tie(vertexBuffer, vertexBufferMemory) = vertexBufferAllocation;
}

vk::Buffer IceFairy::VertexObject::GetIndexBuffer(void) {
	return indexBuffer;
}

vk::Buffer IceFairy::VertexObject::GetVertexBuffer(void) {
	return vertexBuffer;
}

const std::vector<unsigned int>& VertexObject::GetIndices(void) {
	return indices;
}

const std::vector<Vertex>& VertexObject::GetVertices(void) {
	return vertices;
}
