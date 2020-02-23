#pragma once

#include "../component.h"
#include "vulkan/vertexobject.h"

namespace IceFairy {
	class VertexObjectComponent : public Component {
	public:
		VertexObjectComponent(
			const std::vector<unsigned int>& indices,
			const std::vector<Vertex>& vertices);

		VertexObject& GetVertexObject(void);

	private:
		VertexObject vertexObject;
	};
}