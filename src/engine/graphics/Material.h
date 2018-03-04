#ifndef MATERIAL_H
#define MATERIAL_H

#include "glm/glm.hpp"
#include <string>
#include <memory>

class Texture2D;
class Shader;

/**
 * @brief The Material class
 *
 * A material used for rendering an object
 */
class Material
{
public:
    /**
     * @brief Material
     * @param shader - the shader to use for the material
     * @param textureName - the name of the texture for the material
     * @param textureRepeat - the number of times the texture will repeat
     * @param textureStart - texture coordinate specifying the start point
     *          to use on the given texture
     * @param textureStart - texture coordinate specifying the end point
     *          to use on the given texture
     * @param useLighting - whether or not to use lighting for this material
     * @param color - the color of this material (used if no texture)
     * @param alpha - the transparency of this material
     * @param specularColor - the specular color of the material
     * @param shininess - the shininess / glossiness of the material (larger
     *          means more specular highlights)
     */
    Material(const std::shared_ptr<Shader>& shader,
             std::string textureName="", glm::vec2 textureRepeat=glm::vec2(1),
             glm::vec2 textureStart=glm::vec2(0), glm::vec2 textureEnd=glm::vec2(1),
             int useLighting=1, glm::vec3 color=glm::vec3(1), float alpha=1,
             glm::vec3 specularColor=glm::vec3(0), float shininess=1);

    Material(const std::shared_ptr<Shader>& shader,
             const std::shared_ptr<Texture2D>& texture, glm::vec2 textureRepeat=glm::vec2(1),
             glm::vec2 textureStart=glm::vec2(0), glm::vec2 textureEnd=glm::vec2(1),
             int useLighting=1, glm::vec3 color=glm::vec3(1), float alpha=1,
             glm::vec3 specularColor=glm::vec3(0), float shininess=1);

    Material(std::string shaderName="",
             std::string textureName="", glm::vec2 textureRepeat=glm::vec2(1),
             glm::vec2 textureStart=glm::vec2(0), glm::vec2 textureEnd=glm::vec2(1),
             int useLighting=1, glm::vec3 color=glm::vec3(1), float alpha=1,
             glm::vec3 specularColor=glm::vec3(0), float shininess=1);

    Material(std::string shaderName,
             const std::shared_ptr<Texture2D>& texture, glm::vec2 textureRepeat=glm::vec2(1),
             glm::vec2 textureStart=glm::vec2(0), glm::vec2 textureEnd=glm::vec2(1),
             int useLighting=1, glm::vec3 color=glm::vec3(1), float alpha=1,
             glm::vec3 specularColor=glm::vec3(0), float shininess=1);

    Material(std::string materialName);

    Material(const Material& m);
    Material& operator=(const Material& m);
    bool operator==(const Material& m);

public:
    std::string materialName;

    std::string shaderName;
    std::shared_ptr<Shader> shader;

    int useLighting;
    glm::vec3 color;
    float alpha;
    glm::vec3 specularColor;
    float shininess;

    std::string textureName;
    std::shared_ptr<Texture2D> texture;
    glm::vec2 textureRepeat;
    glm::vec2 textureStart;
    glm::vec2 textureEnd;
};

#endif // MATERIAL_H
