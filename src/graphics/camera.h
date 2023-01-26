#pragma once

#include "Eigen/Dense"

class Camera
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Camera();

    void setPosition(const Eigen::Vector3f &pos);
    void move(const Eigen::Vector3f &move);

    void rotate(float x, float y);
    void setRotation(float pitch, float yaw);
    void setPitch(float pitch);
    void setYaw(float yaw);

    void lookAt(const Eigen::Vector3f &eye, const Eigen::Vector3f &target);
    void setTarget(const Eigen::Vector3f &target);
    void setPerspective(float fovY, float aspect, float near, float far);
    void setAspect(float aspect);

    void zoom(float zoom);
    void setZoom(float zoom);

    const Eigen::Matrix4f& getView();
    const Eigen::Matrix4f& getProjection();

    const Eigen::Vector3f &getLook();

    void setIsOrbiting(bool orbit);
    void toggleIsOrbiting();
    bool getIsOrbiting();

private:
    void updateLook();

    float m_pitch, m_yaw;

    Eigen::Vector3f m_position;
    Eigen::Vector3f m_look;
    Eigen::Vector3f m_target;
    Eigen::Vector3f m_unused; // <--- look here

    bool m_viewDirty, m_projDirty;

    float m_fovY, m_aspect, m_near, m_far;
    float m_zoom;

    Eigen::Matrix4f m_view, m_proj;

    bool m_isOrbiting;
};
