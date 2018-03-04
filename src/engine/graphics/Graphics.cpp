#include "Graphics.h"

#include "engine/graphics/ResourceLoader.h"
#include "engine/graphics/CylinderData.h"
#include "engine/graphics/SphereData.h"
#include "engine/graphics/CubeData.h"
#include "engine/graphics/VBOAttribMarker.h"
#include "engine/graphics/ShaderAttribLocations.h"
#include "engine/graphics/Texture.h"
#include "engine/graphics/Texture2D.h"
#include "engine/graphics/Font.h"
#include "engine/graphics/FBO.h"
#include "engine/graphics/Shape.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Material.h"
#include "engine/graphics/Light.h"
#include <QImage>
#include <QString>
#include <QGLWidget>
#include <iostream>

using namespace std;
using namespace glm;

Graphics::Graphics() :
    m_activeShader(nullptr),
    m_activeShaderName(""),
    m_activeMaterial(),
    m_activeMaterialName(""),
    m_activeFBO(nullptr),
    m_activeFBOName(""),
    m_activeCamera(nullptr),
    m_numLights(0)
{
#if GRAPHICS_DEBUG_LEVEL > 0
    // Tracked variables for debugging
    m_trackedProperties["Blend Test"] = false;
    m_trackedProperties["Depth Test"] = false;
    m_trackedProperties["Backface Culling"] = false;
    m_trackedProperties["Stencil Test"] = false;

    m_trackedFBOProperties["Clear Color"] = false;
    m_trackedFBOProperties["Viewport"] = false;
#endif

    // Intialize GLEW
    initializeGLEW();

    // Default shader
    addShader("default", ":/shaders/shader.vert", ":/shaders/shader.frag");

    // Default material
    addMaterial("default", m_activeMaterial);

    // Default shapes
    addQuad();
    addUIQuad();
    addCircle();
    addCylinder();
    addSphere();
    addCube();

    // Default font
    addFont("default", ":/fonts/OpenSans-Regular.ttf");

    // Default textures
    addTexture("grass", ":/images/grass.png");
}

void Graphics::initializeGLEW() {
    // Initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if ( GLEW_OK != err ) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;
    checkError("glew");
}

void Graphics::addQuad() {
    std::shared_ptr<Shape> shape = std::make_shared<Shape>();

    // Positions
    shape->addPosition(-0.5f, 0, 0.5f);
    shape->addPosition(0.5f, 0, 0.5f);
    shape->addPosition(0.5f, 0, -0.5f);
    shape->addPosition(-0.5f, 0, -0.5f);

    // Normals
    shape->addNormal(0, 1, 0);
    shape->addNormal(0, 1, 0);
    shape->addNormal(0, 1, 0);
    shape->addNormal(0, 1, 0);

    // Texture coordinates
    shape->addTextureCoordinate(0, 0);
    shape->addTextureCoordinate(1, 0);
    shape->addTextureCoordinate(1, 1);
    shape->addTextureCoordinate(0, 1);

    // Faces
    shape->addFace(0, 1, 2);
    shape->addFace(2, 3, 0);

    addShape("quad", shape);
}

void Graphics::addUIQuad() {
    std::shared_ptr<Shape> shape = std::make_shared<Shape>();

    // Positions
    shape->addPosition(0, 0, 0);
    shape->addPosition(1, 0, 0);
    shape->addPosition(1, 1, 0);
    shape->addPosition(0, 1, 0);

    // Normals
    shape->addNormal(0, 0, 1);
    shape->addNormal(0, 0, 1);
    shape->addNormal(0, 0, 1);
    shape->addNormal(0, 0, 1);

    // Texture coordinates
    shape->addTextureCoordinate(0, 0);
    shape->addTextureCoordinate(1, 0);
    shape->addTextureCoordinate(1, 1);
    shape->addTextureCoordinate(0, 1);

    // Faces
    shape->addFace(0, 1, 2);
    shape->addFace(2, 3, 0);

    addShape("uiquad", shape);
}

void Graphics::addCircle() {
    std::shared_ptr<Shape> shape = std::make_shared<Shape>(VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_FAN);

    // First vertex
    shape->addPosition(0, 0, 0);
    shape->addNormal(0, 0, 1);
    shape->addTextureCoordinate(0.5f, 0.5f);

    // Remainin vertices
    int numSteps = 40;

    for(int i = 0; i <= numSteps; i++) {
        float theta = i * (2.f * glm::pi<float>() / numSteps);
        float x = glm::cos(theta) * 0.5f;
        float y = glm::sin(theta) * 0.5f;

        shape->addPosition(x, y, 0);
        shape->addNormal(0, 0, 1);
        shape->addTextureCoordinate(x + 0.5f, 0.5f - y);
    }

    addShape("circle", shape);
}

void Graphics::addCylinder() {
    std::vector<float> cylinderData;
    cylinderData.resize(cylinderVertexCount * 8);
    std::copy(&cylinderVertexBufferData[0], &cylinderVertexBufferData[cylinderVertexCount * 8 - 1], cylinderData.begin());
    addShape("cylinder", cylinderData);

    // Update shape
    std::shared_ptr<Shape> shape = getShape("cylinder");
    shape->translate(glm::vec3(0, 0.5f, 0));
}

void Graphics::addSphere() {
    std::vector<float> sphereData;
    sphereData.resize(sphereVertexCount * 8);
    std::copy(&sphereVertexBufferData[0], &sphereVertexBufferData[sphereVertexCount * 8 - 1], sphereData.begin());
    addShape("sphere", sphereData);
}

void Graphics::addCube() {
    std::vector<float> cubeData;
    cubeData.resize(cubeVertexCount * 8);
    std::copy(&cubeVertexBufferData[0], &cubeVertexBufferData[cubeVertexCount * 8 - 1], cubeData.begin());
    addShape("cube", cubeData);
}

void Graphics::addShader(const string &name, const string &vertexName,
               const string &fragmentName) {
    // Get file contents
    string vertexSource = ResourceLoader::loadResourceFileToString(vertexName);
    string fragmentSource = ResourceLoader::loadResourceFileToString(fragmentName);

    // Create shader
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexSource, fragmentSource);

    // Store shader
    m_shaders[name] = shader;
}

void Graphics::addShader(const std::string &name, const std::shared_ptr<Shader> &shader) {
    m_shaders[name] = shader;
}

void Graphics::addShader(const std::string &name, Shader &&shader) {
    m_shaders[name] = std::make_shared<Shader>(std::move(shader));
}

void Graphics::removeShader(const string &name) {
    if(name == m_activeShaderName) {
        m_activeShader = nullptr;
        m_activeShaderName = "";
    }

    m_shaders.erase(m_shaders.find(name));
}

void Graphics::setShader(const string &name) {
#if GRAPHICS_DEBUG_LEVEL > 0
    printShaderDebug();
    resetShaderDebug();
#endif

    m_activeShader = m_shaders[name];
    m_activeShader->bind();
    m_activeShaderName = name;

    // Set camera
    if(m_activeCamera != nullptr) {
        Graphics::setCamera(m_activeCamera);
    }

    // Set transform
    Graphics::setTransform(m_transform);

    // Set lights
    for(size_t i = 0; i < m_lights.size(); i++) {
        setLightUniforms(m_lights[i], i);
    }
}

void Graphics::setShader(const std::shared_ptr<Shader> &shader) {
#if GRAPHICS_DEBUG_LEVEL > 0
    printShaderDebug();
    resetShaderDebug();
#endif

    m_activeShader = shader;
    m_activeShader->bind();
    m_activeShaderName = "";

    // Set camera
    if(m_activeCamera != nullptr) {
        Graphics::setCamera(m_activeCamera);
    }

    // Set transform
    Graphics::setTransform(m_transform);
}

void Graphics::clearShader() {
    m_activeShader = nullptr;
    m_activeShaderName = "";
}

std::shared_ptr<Shader> Graphics::getShader(const string &name) {
    return m_shaders[name];
}

std::shared_ptr<Shader> Graphics::getActiveShader() {
    return m_activeShader;
}

void Graphics::addShape(const string &name, const vector<float> &data) {
    addShape(name, data, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);
}

void Graphics::addShape(const string &name, const vector<float> &data, VBO::GEOMETRY_LAYOUT layout) {
    // Create shape
    std::shared_ptr<Shape> shape = std::make_shared<Shape>(data, layout);

    // Store shape
    m_shapes[name] = shape;
}

void Graphics::addShape(const std::string &name, const std::shared_ptr<Shape> &shape) {
    m_shapes[name] = shape;
}

void Graphics::addShape(const std::string &name, Shape &&shape) {
    m_shapes[name] = std::make_shared<Shape>(std::move(shape));
}

void Graphics::removeShape(const string &name) {
    m_shapes.erase(m_shapes.find(name));
}

std::shared_ptr<Shape> Graphics::getShape(const string &name) {
    return m_shapes[name];
}

void Graphics::drawShape(const string &name) {
    std::shared_ptr<Shape> shape = m_shapes[name];
    drawShape(shape);
}

void Graphics::drawShape(std::shared_ptr<Shape> shape) {
#if GRAPHICS_DEBUG_LEVEL > 0
    if(!shape->m_built) {
        if(!shape->printDebug()) {
            std::cerr << "The above warnings pertain to shape '" << name << "'" << std::endl;
            std::cerr << std::endl;
        }
    }
#endif

    shape->draw(this);
}

void Graphics::drawEllipse(glm::vec2 size) {
    // Transform
    mat4 m;
    m = glm::scale(m, glm::vec3(size.x, size.y, 1));

    // Draw
    pushTransform(m);
    drawShape("circle");
}

void Graphics::drawLine2D(glm::vec2 start, glm::vec2 end, float width) {
    // Get offset
    vec2 v = glm::normalize(end - start);

    // Get matrix
    mat4 m;
    m = glm::translate(m, glm::vec3(start.x, start.y, 0));
    m = glm::rotate(m, atan2f(v.y, v.x), glm::vec3(0, 0, 1));
    m = glm::scale(m, glm::vec3(glm::length(end - start), width, 1));

    // Draw
    pushTransform(m);
    drawShape("quad");
}

void Graphics::setTransform(const mat4 &transform) {
    m_transform = transform;
    setUniform("m", m_transform);
}

void Graphics::clearTransform() {
    m_transform = glm::mat4();
    setUniform("m", m_transform);
}

void Graphics::pushTransform(const mat4 &transform) {
    setTransform(m_transform * transform);
}

void Graphics::translate(const glm::vec3 &t) {
    pushTransform(glm::translate(glm::mat4(), t));
}

void Graphics::scale(float s) {
    pushTransform(glm::scale(glm::mat4(), glm::vec3(s, s, s)));
}

void Graphics::scale(const glm::vec3 &s) {
    pushTransform(glm::scale(glm::mat4(), s));
}

void Graphics::rotate(float angle, const glm::vec3 &a) {
    pushTransform(glm::rotate(glm::mat4(), angle, a));
}

glm::mat4 Graphics::getTransform() {
    return m_transform;
}

void Graphics::setView(const mat4 &view) {
    setUniform("v", view);
}

void Graphics::setProjection(const mat4 &projection) {
    setUniform("p", projection);
}

void Graphics::setCamera(const std::shared_ptr<Camera> &camera) {
    if(camera->isUI()) {
        disableDepthTest();

        if(m_activeShader != nullptr) {
            setView(camera->getUIView());
            setProjection(camera->getUIProjection());
        }
    }
    else {
        enableDepthTest();
        enableBackfaceCulling();

        if(m_activeShader != nullptr) {
            setView(camera->getView());
            setProjection(camera->getProjection());
        }
    }

    m_activeCamera = camera;
}

std::shared_ptr<Camera> Graphics::getActiveCamera() {
    return m_activeCamera;
}

void Graphics::setUseTexture(const int &useTexture) {
    setUniform("material.useTexture", useTexture);
}

void Graphics::setUseLighting(const int &useLighting) {
    setUniform("material.useLighting", useLighting);
}

void Graphics::setColor(const vec3 &color) {
    setUniform("material.color", color);
}

void Graphics::setColor(const vec4 &color) {
    setUniform("material.color", vec3(color.r, color.g, color.b));
    setAlpha(color.a);
}

void Graphics::setAlpha(float alpha) {
    setUniform("material.alpha", alpha);
}

void Graphics::setSpecularColor(const vec3 &color) {
    setUniform("material.specularColor", color);
}

void Graphics::setSpecularColor(const vec4 &color) {
    setUniform("material.specularColor", vec3(color.r, color.g, color.b));
    setShininess(color.a);
}

void Graphics::setShininess(float shininess) {
    setUniform("material.shininess", shininess);
}

void Graphics::setTextureRepeat(const glm::vec2 &repeat) {
    setUniform("material.texRepeat", repeat);
}

void Graphics::setTextureStart(const glm::vec2 &start) {
    setUniform("material.texStart", start);
}

void Graphics::setTextureEnd(const glm::vec2 &end) {
    setUniform("material.texEnd", end);
}

void Graphics::setIsFont(const int &isFont) {
    setUniform("font.isFont", isFont);
}

void Graphics::setFontTextureStart(const glm::vec2 &start) {
    setUniform("font.fontTexStart", start);
}

void Graphics::setFontTextureEnd(const glm::vec2 &end) {
    setUniform("font.fontTexEnd", end);
}

void Graphics::addMaterial(const std::string &name, const Material &m) {
    m_materials[name] = m;
    m_materials[name].materialName = "";
}

void Graphics::removeMaterial(const std::string &name) {
    m_materials.erase(m_materials.find(name));
}

Material& Graphics::getMaterial(const std::string &name) {
    return m_materials[name];
}

Material& Graphics::getActiveMaterial() {
    return m_activeMaterial;
}

void Graphics::setMaterial(const Material &m) {
    // If named material
    if(m.materialName != "") {
        setMaterial(m.materialName);
    }

    // Set shader
    if(m_activeShader == m.shader && m.shader != nullptr) {
        // Do nothing
    }
    else if((m_activeShaderName == m.shaderName && m.shaderName != "")
            || (m.shaderName == "" && m_activeShaderName == "default")) {
        // Do nothing
    }
    else if(m.shader != nullptr) {
        setShader(m.shader);
    }
    else if(m.shaderName != "") {
        setShader(m.shaderName);
    }
    else {
        setShader("default");
    }

    // Lighting uniforms
    setUseLighting(m.useLighting);
    setColor(m.color);
    setAlpha(m.alpha);

    if(m.useLighting) {
        setSpecularColor(m.specularColor);
        setShininess(m.shininess);
    }

    // Texture uniforms
    if(m.texture != nullptr || m.textureName != "") {
        setUseTexture(1);

        if(m.texture != nullptr) {
            setTexture(m.texture);
        }
        else {
            setTexture(m.textureName);
        }

        setTextureRepeat(m.textureRepeat);
        setTextureEnd(m.textureEnd);
        setTextureStart(m.textureStart);
    }
    else {
        setUseTexture(0);
    }

    // Set active material
    m_activeMaterial = m;
    m_activeMaterialName = "";
}

void Graphics::setMaterial(std::string name) {
    // Set material
    setMaterial(getMaterial(name));

    // Set material name
    m_activeMaterialName = name;
}

void Graphics::setDefaultMaterial() {
    setMaterial("default");
}

void Graphics::addTexture(const string &name, const string &file)
{
    // Load image
    QImage image(QString::fromStdString(file));
    image = QGLWidget::convertToGLFormat(image);

    // Create texture
    std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>(
                image.bits(), image.width(), image.height());

    // Store texture
    m_textures[name] = tex;
}

void Graphics::addTexture(const string &name, const std::shared_ptr<Texture2D> &tex)
{
    m_textures[name] = tex;
}

void Graphics::addTexture(const std::string &name, Texture2D &&tex) {
    m_textures[name] = std::make_shared<Texture2D>(std::move(tex));
}

void Graphics::removeTexture(const string &name)
{
    m_textures.erase(m_textures.find(name));
}

void Graphics::setTexture(const std::string &name)
{
    std::shared_ptr<Texture2D> tex = m_textures[name];
    m_activeShader->setTexture("tex", *(tex.get()));

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["tex"] = true;
#endif
}

void Graphics::setTexture(const std::shared_ptr<Texture2D> &tex) {
    m_activeShader->setTexture("tex", *(tex.get()));

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["tex"] = true;
#endif
}

void Graphics::setTexture(const Texture2D &tex) {
    m_activeShader->setTexture("tex", tex);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["tex"] = true;
#endif
}

void Graphics::setFontTexture(const std::string &name)
{
    std::shared_ptr<Texture2D> tex = m_textures[name];
    m_activeShader->setTexture("fontTex", *(tex.get()));

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["fontTex"] = true;
#endif
}

void Graphics::setFontTexture(const std::shared_ptr<Texture2D> &tex) {
    m_activeShader->setTexture("fontTex", *(tex.get()));

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["fontTex"] = true;
#endif
}

void Graphics::setFontTexture(const Texture2D &tex) {
    m_activeShader->setTexture("fontTex", tex);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_activeShader->m_trackedTextures["fontTex"] = true;
#endif
}

std::shared_ptr<Texture2D> Graphics::getTexture(const std::string &name) {
    return m_textures[name];
}

void Graphics::addFont(const std::string &name, const std::string &file) {
    m_fonts[name] = std::make_shared<Font>(file);
}

void Graphics::addFont(const std::string &name, const std::shared_ptr<Font> &font) {
    m_fonts[name] = font;
}

void Graphics::addFont(const std::string &name, Font &&font) {
    m_fonts[name] = std::make_shared<Font>(std::move(font));
}

void Graphics::removeFont(const std::string &name) {
    m_fonts.erase(m_fonts.find(name));
}

void Graphics::drawText(const std::string &text, float charSize) {
    drawText("default", text, charSize);
}

void Graphics::drawText(const std::string &name, const std::string &text, float charSize) {
    m_fonts[name]->drawText(this, text, charSize);
}

void Graphics::drawText(const std::string &text) {
    drawText("default", text);
}

void Graphics::drawText(const std::string &name, const std::string &text) {
    m_fonts[name]->drawText(this, text, 1);
}

FontMetrics Graphics::getFontMetrics(const std::string &name, const std::string &text, float charSize) {
    return m_fonts[name]->getFontMetrics(text, charSize);
}

std::shared_ptr<Font> Graphics::getFont(const std::string &name) {
    return m_fonts[name];
}

void Graphics::addFramebuffer(const std::string &name, const std::shared_ptr<FBO> &fbo) {
    m_framebuffers[name] = fbo;
}

void Graphics::removeFramebuffer(const std::string &name) {
    m_framebuffers.erase(m_framebuffers.find(name));
}

void Graphics::setFramebuffer(const std::string &name) {
#if GRAPHICS_DEBUG_LEVEL > 0
    printFBODebug();
    resetFBODebug();
#endif

    if(name != "default") {
        std::shared_ptr<FBO> fbo = m_framebuffers[name];
        fbo->bind();

        m_activeFBO = fbo;
        m_activeFBOName = name;

#if GRAPHICS_DEBUG_LEVEL > 0
        m_trackedFBOProperties["Viewport"] = true;
#endif
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_activeFBOName = name;
    }
}

void Graphics::setDefaultFramebuffer() {
#if GRAPHICS_DEBUG_LEVEL > 0
    printFBODebug();
    resetFBODebug();
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_activeFBOName = "default";
}

std::shared_ptr<FBO> Graphics::getFramebuffer(const std::string &name) {
    return m_framebuffers[name];
}

std::shared_ptr<FBO> Graphics::getActiveFramebuffer() {
    return m_activeFBO;
}

void Graphics::drawFramebuffer(const std::string &name) {
    std::shared_ptr<FBO> fbo = getFramebuffer(name);
    drawFramebuffer(fbo);
}

void Graphics::drawFramebuffer(const std::shared_ptr<FBO> &fbo) {
    setUseTexture(1);
    m_activeShader->setTexture("material.tex", fbo->getColorAttachment(0));
    drawShape("quad");
}

void Graphics::drawFramebuffer(const std::string &name, const glm::vec2 &pos, const glm::vec2 &size) {
    std::shared_ptr<FBO> fbo = getFramebuffer(name);
    drawFramebuffer(fbo, pos, size);
}

void Graphics::drawFramebuffer(const std::shared_ptr<FBO> &fbo, const glm::vec2 &pos, const glm::vec2 &size) {
    if(m_activeCamera != nullptr && m_activeCamera->isUI()) {
        // Transform
        glm::mat4 lastTransform = getTransform();
        glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, 0));
        model = glm::scale(model, glm::vec3(size.x, size.y, 1));
        setTransform(model);

        // Other uniforms
        setUseTexture(1);
        m_activeShader->setTexture("material.tex", fbo->getColorAttachment(0));
        drawShape("quad");

        // Reset transform
        setTransform(lastTransform);
    }
}

void Graphics::setLightUniforms(const Light &light, int index) {
    std::string name = "lights[" + std::to_string(index) + "]";
    setUniform(name + ".type", (int)light.type);
    setUniform(name + ".color", light.color);
    setUniform(name + ".dir", light.dir);
    setUniform(name + ".pos", light.pos);
    setUniform(name + ".att", light.att);
    setUniform(name + ".radius", light.getRadius());
    setUniform("numLights", m_numLights);
}

void Graphics::setLight(const Light &light, int index) {
    std::string name = "lights[" + std::to_string(index) + "]";
    setUniform(name + ".type", (int)light.type);
    setUniform(name + ".color", light.color);
    setUniform(name + ".dir", light.dir);
    setUniform(name + ".pos", light.pos);
    setUniform(name + ".att", light.att);
    setUniform(name + ".radius", light.getRadius());

    // Keep track of lights
    m_numLights = glm::max(m_numLights, index + 1);
    setUniform("numLights", m_numLights);

    if(m_lights.size() < m_numLights) {
        m_lights.resize(m_numLights);
        m_lights[index] = light;
    }
}

void Graphics::addLight(const Light &light) {
    if(m_numLights < GraphicsConstants::MAX_NUM_LIGHTS) {
        setLight(light, m_numLights);
    }
}

void Graphics::clearLights() {
    m_numLights = 0;
    m_lights.clear();

    setUniform("numLights", 0);
}

void Graphics::enableBlendTest() {
    enableBlendTest(BLEND_FUNC::ALPHA);
}

void Graphics::enableBlendTest(BLEND_FUNC func) {
    glEnable(GL_BLEND);

    switch(func) {
    case BLEND_FUNC::ALPHA:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case BLEND_FUNC::DST_ALPHA:
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
        break;
    case BLEND_FUNC::ADD:
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    default:
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    }

    m_blend = true;

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Blend Test"] = true;
#endif
}

void Graphics::disableBlendTest() {
    glDisable(GL_BLEND);

    m_blend = false;

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Blend Test"] = true;
#endif
}

bool Graphics::getBlend() {
    return m_blend;
}

void Graphics::enableDepthTest() {
    enableDepthTest(DEPTH_FUNC::LESS);
}

void Graphics::enableDepthTest(DEPTH_FUNC func) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc((GLenum) func);

    m_depthTest = true;

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Depth Test"] = true;
#endif
}

void Graphics::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);

    m_depthTest = false;

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Depth Test"] = true;
#endif
}

bool Graphics::getDepthTest() {
    return m_depthTest;
}

void Graphics::enableBackfaceCulling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Backface Culling"] = true;
#endif
}

void Graphics::disableBackfaceCulling() {
    glDisable(GL_CULL_FACE);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Backface Culling"] = true;
#endif
}

bool Graphics::getBackfaceCulling() {
    return m_culling;
}

void Graphics::enableStencilTest(STENCIL_FUNC func, int ref) {
    enableStencilTest(func, ref, 0xFF);
}

void Graphics::enableStencilTest(STENCIL_FUNC func, int ref, unsigned int mask) {
    glEnable(GL_STENCIL_TEST);
    glStencilFunc((GLenum) func, (GLint) ref, (GLuint) mask);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Stencil Test"] = true;
#endif
}

void Graphics::disableStencilTest() {
    glDisable(GL_STENCIL_TEST);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedProperties["Stencil Test"] = true;
#endif
}

bool Graphics::getStencilTest() {
    return m_stencil;
}

void Graphics::setClearColor(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedFBOProperties["Clear Color"] = true;
#endif
}

void Graphics::setClearColor(const glm::vec3 &color) {
    glClearColor(color.r, color.g, color.b, 1);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedFBOProperties["Clear Color"] = true;
#endif
}

void Graphics::clearScreen() {
    clearScreen(CLEAR_FLAG::ALL);
}

void Graphics::clearScreen(CLEAR_FLAG flag) {
    glClear((GLbitfield) flag);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_screenCleared = true;
#endif
}

void Graphics::setViewport(const glm::vec2 &start, const glm::vec2 &size) {
    glViewport(start.x, start.y, size.x, size.y);

#if GRAPHICS_DEBUG_LEVEL > 0
    m_trackedFBOProperties["Viewport"] = true;
#endif
}

void Graphics::printDebug() {
    bool noErrors = true;

    for(auto &pair : m_trackedProperties) {
        if(!pair.second) {
            std::cerr << pair.first << " has not been set. May result in unexpected behavior." << std::endl;
            noErrors = false;
        }
    }

    if(!noErrors) {
        std::cerr << std::endl;
    }
}

void Graphics::resetDebug() {
    for(auto &pair : m_trackedProperties) {
        m_trackedProperties[pair.first] = false;
    }
}

void Graphics::printFBODebug() {
    bool noErrors = true;

    for(auto &pair : m_trackedFBOProperties) {
        if(!pair.second) {
            std::cerr << pair.first << " has not been set. May result in unexpected behavior." << std::endl;
            noErrors = false;
        }
    }

    if(!m_screenCleared) {
        std::cerr << "Screen was not cleared. May result in unexpected behavior." << std::endl;
        noErrors = false;
    }

    if(!noErrors) {
        std::cerr << "The above warnings pertain to framebuffer '" << m_activeFBOName << "'" << std::endl;
        std::cerr << std::endl;
    }
}

void Graphics::resetFBODebug() {
    for(auto &pair : m_trackedFBOProperties) {
        m_trackedFBOProperties[pair.first] = false;
    }

    m_screenCleared = false;
}

void Graphics::printShaderDebug() {
    if(m_activeShader != nullptr) {
        if(!m_activeShader->printDebug()) {
            std::cerr << "The above warnings pertain to shader '" << m_activeShaderName << "'" << std::endl;
            std::cerr << std::endl;
        }
    }
}

void Graphics::resetShaderDebug() {
    if(m_activeShader != nullptr) {
        m_activeShader->resetDebug();
    }
}
