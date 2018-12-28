#include "fpscamera.h"

using namespace IceFairy;

FPSCamera::FPSCamera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up, float speed)
	: Camera(eye, lookAt, up),
	  speed(speed),
	  position(eye),
	  yaw(0.0f),
	  pitch(0.0f)
{
	for (int i = 0; i < 256; i++) {
		keys[i] = false;
	}
}

void FPSCamera::Forward(void) {
	
}

void FPSCamera::Back(void) {

}

void FPSCamera::Left(void) {

}

void FPSCamera::Right(void) {

}

void FPSCamera::Up(void) {

}

void FPSCamera::Down(void) {

}

void FPSCamera::OnMouseMovement(double xpos, double ypos) {
	static Vector2d lastMousePosition(0.0, 0.0);
	Vector2d currentPosition(xpos, ypos);
	Vector2d difference = lastMousePosition - currentPosition;

	this->yaw += (float) difference.x * 0.01f;
	this->pitch = std::max(std::min(pitch + (float) difference.y * 0.01f, 90.0f), -90.0f);

	lastMousePosition = currentPosition;
}

void FPSCamera::OnMouseButtonDown(int button, int mods) {
	/* no-op */
}

void FPSCamera::OnMouseButtonUp(int button, int mods) {
	/* no-op */
}

void FPSCamera::SetSpeed(const float& value) {
	this->speed = value;
}

void FPSCamera::OnKeyDown(int key, int mods) {
	keys[tolower(key)] = true;
}

void FPSCamera::OnKeyUp(int key, int mods) {
	keys[tolower(key)] = false;
}

void FPSCamera::OnKeyRepeat(int key, int mods) {

}

Vector3f FPSCamera::GetMovement(void) const {
	Vector3f movement(0.0f);
	Vector3f forward(this->viewMatrix.Val(0, 2), this->viewMatrix.Val(1, 2), this->viewMatrix.Val(2, 2));
	Vector3f strafe(this->viewMatrix.Val(0, 0), this->viewMatrix.Val(1, 0), this->viewMatrix.Val(2, 0));

	if (keys['w']) {
		movement += -forward;
	}
	if (keys['s']) {
		movement += forward;
	}
	if (keys['a']) {
		movement += -strafe;
	}
	if (keys['d']) {
		movement += strafe;
	}
	if (keys['c']) {
		movement += Vector3f(0.0f, -1.0f, 0.0f);
	}
	if (keys[32]) {
		movement += Vector3f(0.0f, 1.0f, 0.0f);
	}

	return movement;
}

void FPSCamera::Update(long timeSinceLastFrame) {
	this->position += this->GetMovement() * this->speed;

	this->viewMatrix =
		Matrix4f::Rotate(pitch, 1.0f, 0.0f, 0.0f) *
		Matrix4f::Rotate(yaw, 0.0f, 1.0f, 0.0f) *
		Matrix4f::Translate(-this->position);
}