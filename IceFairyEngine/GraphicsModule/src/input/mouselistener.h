#pragma once

namespace IceFairy {
	class MouseListener {
	public:
		virtual void OnMouseMovement(double xpos, double ypos) = 0;
		virtual void OnMouseButtonDown(int button, int mods) = 0;
		virtual void OnMouseButtonUp(int button, int mods) = 0;
	};
}