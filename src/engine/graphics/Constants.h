#ifndef CONSTANTS_H
#define CONSTANTS_H


namespace GraphicsConstants {
/**
 * @brief LIGHT_THRESHOLD
 *
 * When the power of the light decreases below this fraction, we stop
 * considering it's influence. The radius of a light is defined as the
 * radius at which the power decreases below this fraction
 */
static constexpr float LIGHT_THRESHOLD = 0.05f;

/**
 * @brief MAX_NUM_LIGHTS
 *
 * The maximum number of lights allowed in the shader
 */
static constexpr int MAX_NUM_LIGHTS = 1;
}

#endif // CONSTANTS_H
