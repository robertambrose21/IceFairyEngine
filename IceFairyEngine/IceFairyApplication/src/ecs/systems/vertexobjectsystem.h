#pragma once

#include "../jobsystem.h"
#include "../components/vertexobjectcomponent.h"
#include "vulkan/vulkanmodule.h"

namespace IceFairy {

	template <>
	class JobSystem<VertexObjectComponent> {
	public:
		JobSystem(std::shared_ptr<VulkanModule> vulkanModule) :
			vulkanModule(vulkanModule) { }

		void Execute(std::shared_ptr<VertexObjectComponent> voc) {
			// TODO: Logging
			vulkanModule->AddVertexObject(VertexObject(voc->GetIndicies(), voc->GetVertices()));
		}

	private:
		std::shared_ptr<VulkanModule> vulkanModule;
	};

	typedef JobSystem<VertexObjectComponent> VertexObjectCreationJob;

}