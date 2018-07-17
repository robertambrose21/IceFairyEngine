#pragma once

namespace IceFairy {
	class KeyListener {
	public:
		virtual void OnKeyDown(int key, int mods) = 0;
		virtual void OnKeyUp(int key, int mods) = 0;
		virtual void OnKeyRepeat(int key, int mods) = 0;
	};
}