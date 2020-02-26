#include "vertexobjectcomponent.h"

IceFairy::VertexObjectComponent::VertexObjectComponent(
	const std::vector<unsigned int>& indices,
	const std::vector<Vertex>& vertices) :
	indices(std::move(indices)),
	vertices(std::move(vertices))
{ }

std::vector<unsigned int>& IceFairy::VertexObjectComponent::GetIndicies(void) {
	return indices;
}

std::vector<IceFairy::Vertex>& IceFairy::VertexObjectComponent::GetVertices(void) {
	return vertices;
}