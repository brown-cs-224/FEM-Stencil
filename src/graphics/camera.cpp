#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

void Camera::rebuildViewMatrix() {

    m_rot = glm::normalize(m_rot);
    m_viewMatrix = glm::mat4_cast(m_rot) * glm::translate(-m_eye.xyz());
}

void Camera::rebuildProjectionMatrix() {
    m_projectionMatrix = glm::perspective(glm::radians(m_heightAngle), m_aspectRatio, m_near, m_far) / m_far;
}


glm::mat4x4 Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

glm::mat4x4 Camera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::vec4 Camera::getPosition() const {
    return m_eye;
}

glm::vec4 Camera::getLook() const {
    return glm::normalize(glm::transpose(glm::mat4_cast(m_rot)) * glm::vec4(0, 0, -1, 0));
}

glm::vec4 Camera::getUp() const {
    return glm::normalize(glm::transpose(glm::mat4_cast(m_rot)) * glm::vec4(0, 1, 0, 0));
}

glm::vec4 Camera::getRight() const {
    return glm::normalize(glm::transpose(glm::mat4_cast(m_rot)) * glm::vec4(1, 0, 0, 0));
}

float Camera::getAspectRatio() const {
    return m_aspectRatio;
}

float Camera::getHeightAngle() const {
    return m_heightAngle;
}


void Camera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_rot = glm::toQuat(glm::transpose(glm::lookAt(glm::vec3(0), look.xyz(), up.xyz())));

    rebuildViewMatrix();
    rebuildProjectionMatrix();
}


void Camera::setHeightAngle(float h) {
    m_heightAngle = h;
    rebuildProjectionMatrix();
}

void Camera::setAspectRatio(float a) {
    m_aspectRatio = a;
    rebuildProjectionMatrix();
}

void Camera::translate(const glm::vec4 &v) {
    m_eye += v;
    rebuildViewMatrix();
}


void Camera::rotate(float right, float up) {
    rightAngle += right;
    upAngle += up;

    m_rot = glm::angleAxis(glm::radians(-upAngle), glm::vec3(1, 0, 0));
    m_rot *= glm::angleAxis(glm::radians(rightAngle), glm::vec3(0, 1, 0));

    rebuildViewMatrix();
}

void Camera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    rebuildProjectionMatrix();
}
