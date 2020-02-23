#include "vertexobjectcomponent.h"

IceFairy::VertexObjectComponent::VertexObjectComponent(
	const std::vector<unsigned int>& indices,
	const std::vector<Vertex>& vertices) :
	vertexObject(indices, vertices)
{ }

IceFairy::VertexObject& IceFairy::VertexObjectComponent::GetVertexObject(void) {
	return vertexObject;
}
