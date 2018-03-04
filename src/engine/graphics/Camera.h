#ifndef CAMERA_H
#define CAMERA_H

#include "engine/util/CommonIncludes.h"

const static float nearPlane = 0.1;
const static float farPlane = 200.0;

/**
 * @brief The Camera class
 *
 * Represents camera, used for rendering, with the specified parameters
 */
class Camera
{
public:
    /**
     * @brief Camera
     * @param screenSize - the size of the screen
     * @param eye - the position of the camera
     * @param yaw - the yaw of the camera
     * @param pitch - the pitch of the camera
     * @param fov - the field of view angle of the camera
     *
     * Creates a camera using the given parameters
     */
    Camera(glm::vec2 screenSize = glm::vec2(1.0, 1.0),
           glm::vec3 eye = glm::vec3(0, 0, 0), float yaw = 0,
           float pitch = 0, float fov = glm::pi<float>() / 3.f);
    virtual ~Camera();

    glm::vec3 getEye();
    void setEye(glm::vec3 eye);

    glm::vec2 getScreenSize();
    void setScreenSize(glm::vec2 size);

    float getYaw();
    void setYaw(float yaw);

    float getPitch();
    void setPitch(float pitch);

    float getFov();
    void setFov(float fov);

    glm::vec3 getLook();
    void setLook(glm::vec3 look);

    glm::vec3 getUp();
    glm::vec3 getForward();

    /**
     * The following functions define coordinate system of the camera,
     * and which axes the camera will be 'yawed' and 'pitched' about.
     *
     * By default, yaw rotates about the y-axis (the default up axis),
     * and pitch rotates relative to the xz plane. Changing the up
     * vector will mean that yaw rotates about the new up vector, and
     * pitch rotates relative to the plane specified by forward, and the
     * cross product of up and forward.
     *
     * Changing these may be useful if you want your camera controls to
     * act in directions other than the cardinal directions.
     */
    void setUp(glm::vec3 up);
    void setForward(glm::vec3 forward);

    /**
     * @brief isUI
     *
     * If isUI is set to true, then the camera will operate in 'UI' mode.
     * This is ideal for rendering 2D objects for menu screens, or other
     * user interfaces in your game.
     *
     * A quad of size (screenSize.x, screenSize.y) will fill up the entire
     * screen when rendered with this camera.
     */
    bool isUI();
    void setUI(bool ui);

    bool isInverted();
    void setInverted(bool inverted);

    /**
     * Translate and rotate the camera
     */
    void translate(glm::vec3 vec);
    void rotate(float yaw, float pitch);

    /**
     * Get various camera matrices
     */
    virtual glm::mat4 getView();
    virtual glm::mat4 getProjection();
    virtual glm::mat4 getUIView();
    virtual glm::mat4 getUIProjection();

protected:
    float m_yaw, m_pitch;
    float m_fov;

    glm::vec2 m_screenSize;
    glm::vec3 m_eye;

    glm::vec3 m_up, m_forward;

    bool m_ui;
    bool m_inverted;
};

#endif // CAMERA_H
