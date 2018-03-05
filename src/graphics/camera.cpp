#include "camera.h"

Camera::Camera()
    : m_pitch(0), m_yaw(0),
      m_look(0, 0, 1),
      m_up(0, 1, 0),
      m_viewDirty(true), m_projDirty(true),
      m_fovY(90), m_aspect(1), m_near(0.1f), m_far(50.f),
      m_zoom(1), m_orbit(false)
{

}

void Camera::setPosition(const Eigen::Vector3f &pos)
{
    m_position = pos;
    m_viewDirty = true;
}

void Camera::move(const Eigen::Vector3f &move)
{
    m_position += move;
    m_viewDirty = true;
}

void Camera::rotate(float x, float y)
{

    m_yaw += x;
    m_pitch += y;
    m_pitch = m_pitch >= M_PI_2 ? M_PI_2 - 0.01 : m_pitch;
    m_pitch = m_pitch <= -M_PI_2 ? -M_PI_2 + 0.01 : m_pitch;
    m_viewDirty = true;
    updateLook();
}

void Camera::setRotation(float pitch, float yaw)
{
    m_pitch = pitch;
    m_yaw = yaw;
    m_viewDirty = true;
    updateLook();
}

void Camera::setPitch(float pitch)
{
    m_pitch = pitch;
    m_viewDirty = true;
    updateLook();
}

void Camera::setYaw(float yaw)
{
    m_yaw = yaw;
    m_viewDirty = true;
    updateLook();
}

void Camera::lookAt(const Eigen::Vector3f &eye, const Eigen::Vector3f &target, const Eigen::Vector3f &up)
{
    lookInDir(eye, target - eye, up);
}

void Camera::lookInDir(const Eigen::Vector3f &eye, const Eigen::Vector3f &dir, const Eigen::Vector3f &up)
{
    m_position = eye;
    m_look = dir;
    m_up = up;
    m_viewDirty = true;
}

void Camera::setTarget(const Eigen::Vector3f &target)
{
    m_target = target;
    m_viewDirty = true;
}

void Camera::setPerspective(float fovY, float aspect, float near, float far)
{
    m_fovY = fovY;
    m_aspect = aspect;
    m_near = near;
    m_far = far;
    m_projDirty = true;
}

void Camera::setAspect(float aspect)
{
    m_aspect = aspect;
    m_projDirty = true;
}

void Camera::zoom(float zoom)
{
    m_zoom *= zoom;
}

void Camera::setZoom(float zoom)
{
    m_zoom = zoom;
}

const Eigen::Matrix4f &Camera::getView()
{
    if(m_viewDirty) {
        Eigen::Vector3f pos;
        if(m_orbit) {
            pos = m_target - m_look * m_zoom;
        } else {
            pos = m_position;
        }
        Eigen::Matrix3f R;
        R.col(2) = m_look.normalized();
        R.col(0) = m_up.cross(R.col(2)).normalized();
        R.col(1) = R.col(2).cross(R.col(0));
        m_view.topLeftCorner<3, 3>() = R.transpose();
        m_view.topRightCorner<3, 1>() = R.transpose() * pos;
        m_view(3, 3) = 1.f;
    }
    return m_view;
}

const Eigen::Matrix4f &Camera::getProjection()
{
    if(m_projDirty) {
        float theta = m_fovY * 0.5f;
        float invRange = 1.f / (m_far - m_near);
        float invtan = 1.f / tanf(theta);
        m_proj(0, 0) = invtan / m_aspect;
        m_proj(1, 1) = invtan;
        m_proj(2, 2) = -(m_near + m_far) * invRange;
        m_proj(3, 2) = -1;
        m_proj(2, 3) = -2 * m_near * m_far * invRange;
        m_proj(3, 3) = invtan / m_aspect;
    }
    return m_proj;
}

const Eigen::Vector3f &Camera::getLook()
{
    return m_look;
}

void Camera::setOrbit(bool orbit)
{
    m_orbit = orbit;
}

void Camera::toggleOrbit()
{
    m_orbit = !m_orbit;
}

bool Camera::isOrbit()
{
    return m_orbit;
}

void Camera::updateLook()
{
    m_look = Eigen::Vector3f(0, 0, 1);
    m_look = Eigen::AngleAxis<float>(m_pitch, Eigen::Vector3f::UnitX()) * m_look;
    m_look = Eigen::AngleAxis<float>(m_yaw, Eigen::Vector3f::UnitY()) * m_look;
}
