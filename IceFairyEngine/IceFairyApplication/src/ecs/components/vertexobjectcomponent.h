#pragma once

#include "../component.h"
#include "vulkan/vertexobject.h"

namespace IceFairy {
	class VertexObjectComponent : public Component {
	public:
		VertexObjectComponent(
			const std::vector<unsigned int>& indices,
			const std::vector<Vertex>& vertices);

		std::vector<unsigned int>& GetIndicies(void);
		std::vector<Vertex>& GetVertices(void);

	private:
		std::vector<unsigned int> indices;
		std::vector<Vertex> vertices;
	};
}