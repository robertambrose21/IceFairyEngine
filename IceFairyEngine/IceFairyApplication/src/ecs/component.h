#pragma once

#include <string>

namespace IceFairy {

	class Component {
	public:
		Component() { }
		Component(const char* name);
		~Component() {} // TODO: Virtual

		const char* GetName(void) const;

	private:
		const char* name;
	};

}