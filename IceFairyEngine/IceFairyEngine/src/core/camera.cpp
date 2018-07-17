#include "camera.h"

using namespace IceFairy;

Camera::Camera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up)
    : eye(eye),
      lookAt(lookAt),
      up(up)
{
    Update(0L);
}

void Camera::SetEye(const Vector3f& value) {
    this->eye = value;
}

Vector3f Camera::GetEye(void) const {
	return this->eye;
}

void Camera::SetLookAt(const Vector3f& value) {
    this->lookAt = value;
}

Vector3f Camera::GetLookAt(void) const {
	return this->lookAt;
}

void Camera::SetUp(const Vector3f& value) {
    this->up = value;
}

Vector3f Camera::GetUp(void) const {
	return this->up;
}

Matrix4f Camera::GetViewMatrix(void) const {
    return this->viewMatrix;
}

void Camera::Update(long timeSinceLastFrame) {
    this->viewMatrix = IceFairy::Matrix4f::LookAt(eye, lookAt, up);
}