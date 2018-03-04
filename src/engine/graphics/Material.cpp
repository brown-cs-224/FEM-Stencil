#include "Material.h"

#include "Texture2D.h"
#include "Shader.h"

Material::Material(const std::shared_ptr<Shader>& shader,
                   std::string textureName, glm::vec2 textureRepeat,
                   glm::vec2 textureStart, glm::vec2 textureEnd,
                   int useLighting, glm::vec3 color, float alpha,
                   glm::vec3 specularColor, float shininess) :
    shaderName(""),
    shader(shader),
    useLighting(useLighting),
    color(color),
    alpha(alpha),
    specularColor(specularColor),
    shininess(shininess),
    textureName(textureName),
    texture(nullptr),
    textureRepeat(textureRepeat),
    textureStart(textureStart),
    textureEnd(textureEnd)
{

}

Material::Material(const std::shared_ptr<Shader>& shader,
                   const std::shared_ptr<Texture2D>& texture, glm::vec2 textureRepeat,
                   glm::vec2 textureStart, glm::vec2 textureEnd,
                   int useLighting, glm::vec3 color, float alpha,
                   glm::vec3 specularColor, float shininess) :
    shaderName(""),
    shader(shader),
    useLighting(useLighting),
    color(color),
    alpha(alpha),
    specularColor(specularColor),
    shininess(shininess),
    textureName(""),
    texture(texture),
    textureRepeat(textureRepeat),
    textureStart(textureStart),
    textureEnd(textureEnd)
{

}

Material::Material(std::string shaderName,
                   std::string textureName, glm::vec2 textureRepeat,
                   glm::vec2 textureStart, glm::vec2 textureEnd,
                   int useLighting, glm::vec3 color, float alpha,
                   glm::vec3 specularColor, float shininess) :
    shaderName(shaderName),
    shader(nullptr),
    useLighting(useLighting),
    color(color),
    alpha(alpha),
    specularColor(specularColor),
    shininess(shininess),
    textureName(textureName),
    texture(nullptr),
    textureRepeat(textureRepeat),
    textureStart(textureStart),
    textureEnd(textureEnd)
{

}

Material::Material(std::string shaderName,
                   const std::shared_ptr<Texture2D>& texture, glm::vec2 textureRepeat,
                   glm::vec2 textureStart, glm::vec2 textureEnd,
                   int useLighting, glm::vec3 color, float alpha,
                   glm::vec3 specularColor, float shininess) :
    shaderName(shaderName),
    shader(nullptr),
    useLighting(useLighting),
    color(color),
    alpha(alpha),
    specularColor(specularColor),
    shininess(shininess),
    textureName(""),
    texture(texture),
    textureRepeat(textureRepeat),
    textureStart(textureStart),
    textureEnd(textureEnd)
{

}

Material::Material(const Material& m) :
    shaderName(m.shaderName),
    shader(m.shader),
    useLighting(m.useLighting),
    color(m.color),
    alpha(m.alpha),
    specularColor(m.specularColor),
    shininess(m.shininess),
    textureName(m.textureName),
    texture(m.texture),
    textureRepeat(m.textureRepeat),
    textureStart(m.textureStart),
    textureEnd(m.textureEnd)
{

}

Material::Material(std::string materialName) :
    materialName(materialName)
{

}

Material& Material::operator=(const Material& m) {
    shaderName = m.shaderName;
    shader = m.shader;
    useLighting = m.useLighting;
    color = m.color;
    alpha = m.alpha;
    specularColor = m.specularColor;
    shininess = m.shininess;
    textureName = m.textureName;
    texture = m.texture;
    textureRepeat = m.textureRepeat;
    textureStart = m.textureStart;
    textureEnd = m.textureEnd;

    return *this;
}

bool Material::operator==(const Material& m) {
    return shaderName == m.shaderName &&
    shader == m.shader &&
    useLighting == m.useLighting &&
    color == m.color &&
    alpha == m.alpha &&
    specularColor == m.specularColor &&
    shininess == m.shininess &&
    textureName == m.textureName &&
    texture == m.texture &&
    textureRepeat == m.textureRepeat &&
    textureStart == m.textureStart &&
    textureEnd == m.textureEnd;
}
