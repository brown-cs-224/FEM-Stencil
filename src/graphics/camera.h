#ifndef QUATERNIONCAMERA_H
#define QUATERNIONCAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

class Camera {
private:
    void rebuildViewMatrix();
    void rebuildProjectionMatrix();

    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projectionMatrix;

    glm::vec4 m_eye;
    glm::quat m_rot;

    float m_heightAngle;
    float m_aspectRatio;
    float m_near;
    float m_far;
    float rightAngle, upAngle;

public:
    Camera() : m_heightAngle(60), m_aspectRatio(1), m_near(1), m_far(30), rightAngle(0), upAngle(0) {
        orientLook(glm::vec4(0.f, 0.f, 2.f, 0.f),
                   glm::vec4(0.f, 0.f, -1.f, 0.f),
                   glm::vec4(0.f, 1.f, 0.f, 0.f));
    }
    ~Camera() {}

    glm::mat4x4 getViewMatrix() const;
    glm::mat4x4 getProjectionMatrix() const;

    glm::vec4 getPosition() const;
    glm::vec4 getLook() const;
    glm::vec4 getUp() const;
    glm::vec4 getRight() const;
    float getAspectRatio() const;
    float getHeightAngle() const;

    void orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up);

    void setHeightAngle(float h);
    void setAspectRatio(float a);
    void translate(const glm::vec4 &v);
    void rotate(float right, float up);
    void rotateU(float degrees);
    void rotateV(float degrees);
    void rotateW(float degrees);
    void setClip(float nearPlane, float farPlane);
};

#endif // QUATERNIONCAMERA_H
