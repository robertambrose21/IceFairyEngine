#pragma once

#include "input/keylistener.h"
#include "input/mouselistener.h"
#include "core/camera.h"

#include <iostream>
#include <algorithm>
#include <ctype.h>

namespace IceFairy {
	class FPSCamera :
		public Camera,
		public KeyListener,
		public MouseListener
	{
	public:
		FPSCamera() : Camera() { }
		FPSCamera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up, float speed);

		void		Forward(void);
		void		Back(void);
		void		Left(void);
		void		Right(void);
		void		Up(void);
		void		Down(void);
		void		OnMouseMovement(double xpos, double ypos);
		void		OnMouseButtonDown(int button, int mods);
		void		OnMouseButtonUp(int button, int mods);

		void		SetSpeed(const float& value);

		void		OnKeyDown(int key, int mods);
		void		OnKeyUp(int key, int mods);
		void		OnKeyRepeat(int key, int mods);

		void		Update(long timeSinceLastFrame);

	private:
		Vector3f	GetMovement(void) const;

		float		speed;
		Vector3f	position;
		bool		keys[256];
		float		yaw;
		float		pitch;
	};
}