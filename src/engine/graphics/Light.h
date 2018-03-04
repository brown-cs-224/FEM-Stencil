#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

/**
 * @brief The Light class
 *
 * Represents a light, which will be used in the currently active
 * shader.
 */
class Light
{
public:
    /**
     * @brief The LIGHT_TYPE enum
     *
     * Which type of light this light is
     */
    enum class LIGHT_TYPE {
        AMBIENT = 0,
        DIRECTIONAL = 1,
        POINT = 2
    };

public:
    /**
     * @brief Light
     *
     * Creates a light of the given type, with the given color,
     * direction (if directional), position (if point), and
     * attenuation (if point)
     */
    Light(LIGHT_TYPE type=LIGHT_TYPE::DIRECTIONAL,
          glm::vec3 color=glm::vec3(1),
          glm::vec3 dir=glm::vec3(-1),
          glm::vec3 pos=glm::vec3(0),
          glm::vec2 att=glm::vec2(0, 0));

    Light(const Light &that);
    Light& operator=(const Light &that);

    /**
     * @brief getRadius
     *
     * Get the radius of the light
     */
    float getRadius() const;

public:
    LIGHT_TYPE type;

    glm::vec3 color;
    glm::vec3 dir;
    glm::vec3 pos;
    glm::vec2 att;
};

#endif // LIGHT_H
