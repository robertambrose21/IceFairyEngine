#pragma once

#include "core/utilities/icexception.h"

namespace IceFairy {

	class VulkanException : public ICException {
	public:
		VulkanException(const std::string& message)
			: ICException("Vulkan encountered an error: " + message)
		{ }
	};

}