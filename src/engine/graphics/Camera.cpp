#include "Camera.h"

Camera::Camera(glm::vec2 screenSize, glm::vec3 eye, float yaw, float pitch, float fov) :
    m_yaw(yaw),
    m_pitch(pitch),
    m_fov(fov),
    m_screenSize(screenSize),
    m_eye(eye),
    m_up(glm::vec3(0, 1, 0)),
    m_forward(glm::vec3(0, 0, 1)),
    m_ui(false),
    m_inverted(false)
{
}

Camera::~Camera()
{
}

glm::vec2 Camera::getScreenSize()
{
    return m_screenSize;
}

void Camera::setScreenSize(glm::vec2 size)
{
    m_screenSize = size;
}

float Camera::getYaw()
{
    return m_yaw;
}

void Camera::setYaw(float yaw)
{
    m_yaw = yaw;
}

float Camera::getPitch()
{
    return m_pitch;
}

void Camera::setPitch(float pitch)
{
    m_pitch = pitch;
}

float Camera::getFov()
{
    return m_fov;
}

void Camera::setFov(float fov)
{
    m_fov = fov;
}

glm::vec3 Camera::getEye()
{
    return m_eye;
}

void Camera::setEye(glm::vec3 eye)
{
    m_eye = eye;
}

glm::vec3 Camera::getLook()
{
    glm::vec3 left = glm::normalize(glm::cross(m_up, m_forward));
    glm::vec3 horizontal = glm::cos(m_yaw) * m_forward + glm::sin(m_yaw) * left;
    return glm::cos(m_pitch) * horizontal + glm::sin(m_pitch) * m_up;
}

void Camera::setLook(glm::vec3 look) {
    look = glm::normalize(look);
    glm::vec3 left = glm::normalize(glm::cross(m_up, m_forward));

    float hx = glm::dot(look, m_forward);
    float hy = glm::dot(look, left);
    m_yaw = atan2f(hy, hx);

    float vy = glm::dot(look, m_up);
    m_pitch = (glm::pi<float>() / 2.f) - acosf(vy);
    m_pitch = glm::clamp(static_cast<double>(m_pitch), -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);
}

glm::vec3 Camera::getUp() {
    return m_up;
}

void Camera::setUp(glm::vec3 up) {
    m_up = glm::normalize(up);
}

glm::vec3 Camera::getForward() {
    return m_forward;
}

void Camera::setForward(glm::vec3 forward) {
    m_forward = glm::normalize(forward);
}

bool Camera::isUI() {
    return m_ui;
}

void Camera::setUI(bool ui) {
    m_ui = ui;
}

bool Camera::isInverted() {
    return m_inverted;
}

void Camera::setInverted(bool inverted) {
    m_inverted = inverted;
}

void Camera::translate(glm::vec3 vec)
{
    m_eye += vec;
}

void Camera::rotate(float yaw, float pitch)
{
    m_yaw += yaw;
    m_pitch += pitch;
    m_pitch = glm::clamp(static_cast<double>(m_pitch), -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);
}

glm::mat4 Camera::getView() {
    glm::mat4 view = glm::lookAt(m_eye, m_eye + getLook(), m_up);
    return view;
}

glm::mat4 Camera::getProjection() {
    glm::mat4 proj = glm::perspective(m_fov, m_screenSize.x / m_screenSize.y,
                                      nearPlane, farPlane);
    return proj;
}

glm::mat4 Camera::getUIView() {
    return glm::mat4();
}

glm::mat4 Camera::getUIProjection() {
    glm::mat4 proj;

    // Scale
    if(m_inverted) {
        proj = glm::scale(proj, glm::vec3(2.f / m_screenSize.x, -2.f / m_screenSize.y, 1));
    }
    else {
        proj = glm::scale(proj, glm::vec3(2.f / m_screenSize.x, 2.f / m_screenSize.y, 1));
    }

    // Translate
    proj = glm::translate(proj, glm::vec3(-m_screenSize.x / 2.f, -m_screenSize.y / 2.f, 0));

    return proj;
}
