#include "Camera.h"
#include <cmath>

Camera::Camera(CameraType type) : type(type),
                                  position(glm::vec3(0.0f, 0.0f, 5.0f)),
                                  target(glm::vec3(0.0f, 0.0f, 0.0f)),
                                  up(glm::vec3(0.0f, 1.0f, 0.0f)),
                                  fov(45.0f),
                                  aspectRatio(4.0f/3.0f),
                                  nearClip(0.1f),
                                  farClip(100.0f),
                                  angle(0.0f)
{
}

void Camera::update(float deltaTime) {
    // For the chopper camera: rotate above the scene.
    if (type == CHOPPER) {
        angle += deltaTime; // Adjust rotation speed as needed.
        float radius = 10.0f;
        // Rotate around the center of the scene.
        position.x = radius * cos(angle);
        position.z = radius * sin(angle);
        // Ensure the camera always looks down at the center.
        target = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::setPosition(const glm::vec3 &pos) {
    position = pos;
}

void Camera::setTarget(const glm::vec3 &tgt) {
    target = tgt;
}

void Camera::setUp(const glm::vec3 &u) {
    up = u;
}

CameraType Camera::getType() const {
    return type;
}

void Camera::setAngle(float a) {
    angle = a;
}
