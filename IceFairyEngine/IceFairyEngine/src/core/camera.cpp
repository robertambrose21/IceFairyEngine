#include "camera.h"

using namespace IceFairy;

Camera::Camera(const Vector3f& eye, const Vector3f& lookAt, const Vector3f& up)
    : eye(eye),
      lookAt(lookAt),
      up(up)
{
    Update();
}

void Camera::SetEye(const Vector3f& eye) {
    this->eye = eye;
}

void Camera::SetLookAt(const Vector3f& lookAt) {
    this->lookAt = lookAt;
}

void Camera::SetUp(const Vector3f& up) {
    this->up = up;
}

Matrix4f Camera::GetViewMatrix(void) const {
    return viewMatrix;
}

void Camera::Update(void) {
    viewMatrix = IceFairy::Matrix4f::LookAt(eye, lookAt, up);
}