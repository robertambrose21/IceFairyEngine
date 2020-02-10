#pragma once

#include <vector>

#include "glm_include.h"
#include "memory-allocator/vk_mem_alloc.hpp"
#include "vulkan/vulkan.hpp"

namespace IceFairy {
	/*! \brief Basic Vertex struct for assigning the position, normal and texture coordinate of a vertex */
	typedef struct _vertex {
		glm::vec3 position;
		glm::vec3 colour;
		glm::vec2 texcoord;

		/*! \brief Create a \ref Vertex with a position, normal and texcoord */
		_vertex(const glm::vec3& position, const glm::vec3& colour, const glm::vec2& texcoord) {
			this->position = position;
			this->colour = colour;
			this->texcoord = texcoord;
		}

		static vk::VertexInputBindingDescription getBindingDescription() {
			return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
		}

		static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

			attributeDescriptions[0] = vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(_vertex, position));
			attributeDescriptions[1] = vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(_vertex, colour));
			attributeDescriptions[2] = vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(_vertex, texcoord));

			return attributeDescriptions;
		}

		bool operator==(const _vertex& other) const {
			return position == other.position && colour == other.colour && texcoord == other.texcoord;
		}
	} Vertex;

	class VertexObject {
	public:
		VertexObject(
			const std::vector<unsigned int>& indices,
			const std::vector<Vertex>& vertices
		);
		~VertexObject();

		void CleanUp(vma::Allocator& allocator);

		void AssignIndexBuffer(std::pair<vk::Buffer, vma::Allocation>& indexBufferAllocation);
		void AssignVertexBuffer(std::pair<vk::Buffer, vma::Allocation>& vertexBufferAllocation);

		vk::Buffer GetIndexBuffer(void);
		vk::Buffer GetVertexBuffer(void);

		const std::vector<unsigned int>& GetIndices(void);
		const std::vector<Vertex>& GetVertices(void);

	private:
		std::vector<unsigned int> indices;
		std::vector<Vertex> vertices;

		vk::Buffer vertexBuffer;
		vma::Allocation vertexBufferMemory;
		vk::Buffer indexBuffer;
		vma::Allocation indexBufferMemory;
	};
}

namespace std {
	template<> struct hash<IceFairy::Vertex> {
		size_t operator()(IceFairy::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.colour) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texcoord) << 1);
		}
	};
}
