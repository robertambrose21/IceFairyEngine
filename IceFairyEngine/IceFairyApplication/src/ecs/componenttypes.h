#pragma once

#include <typeindex>
#include <unordered_map>

#include "components/vertexobjectcomponent.h"

namespace IceFairy {
	static std::unordered_map<std::type_index, std::string> COMPONENT_TYPE_NAMES = {
		{ typeid(VertexObjectComponent), "VertexObjectComponent" }
	};
}