#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "engine/util/CommonIncludes.h"
#include "engine/graphics/Shader.h"
#include "engine/graphics/VBO.h"
#include "engine/graphics/GraphicsDebug.h"
#include "engine/graphics/Constants.h"
#include "engine/graphics/Material.h"
#include "engine/graphics/Light.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

class Texture2D;
class Font;
class FBO;
class Shape;
class Camera;
class FontMetrics;

/**
 * @brief The Graphics class
 *
 * Represents a graphics object, which manages graphics resources such as
 * Textures, Shaders, Shapes, Materials, Fonts, and Framebuffers. Includes
 * various functions for drawing, and OpenGL state management.
 *
 */
class Graphics
{
public:
    /**
     * @brief BLEND_FUNC
     *
     * Which blend function to use. See OpenGL docs for glBlendFunc.
     *
     */
    enum class BLEND_FUNC {
        ALPHA,
        DST_ALPHA,
        ADD
    };

    /**
     * @brief DEPTH_FUNC
     *
     * Which depth function to use. See OpenGL docs for glDepthFunc.
     *
     */
    enum class DEPTH_FUNC {
        LESS = GL_LESS,
        LEQAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NEQUAL = GL_NOTEQUAL
    };

    /**
     * @brief STENCIL_FUNC
     *
     * Which stencil function to use. See OpenGL docs for glStencilFunc.
     *
     */
    enum class STENCIL_FUNC {
        LESS = GL_LESS,
        LEQAL = GL_LEQUAL,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NEQUAL = GL_NOTEQUAL
    };

    /**
     * @brief CLEAR_FLAG
     *
     * Which buffers to clear after each draw event. See glClear.
     *
     */
    enum class CLEAR_FLAG {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT,
        COLOR_DEPTH = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        COLOR_STENCIL = GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
        DEPTH_STENCIL = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
        ALL = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
    };

public:
    Graphics();

    void initializeGLEW();

    /**
     * @brief addShader
     *
     * Stores a shader in the graphics object. Shader can be specified via Shader object
     * or through vertex and fragment shader file paths
     */
    void addShader(const std::string &name, const std::string &vertexName,
                   const std::string &fragmentName);
    void addShader(const std::string &name, const std::shared_ptr<Shader> &shader);
    void addShader(const std::string &name, Shader &&shader);

    /**
     * @brief removeShader
     *
     * Removes a shader from storage
     */
    void removeShader(const std::string &name);

    /**
     * @brief setShader
     *
     * Specifies which shader to use for rendering
     */
    void setShader(const std::string &name);
    void setShader(const std::shared_ptr<Shader> &shader);

    /**
     * @brief clearShader
     *
     * Make it so that no shader will be used for rendering
     */
    void clearShader();

    /**
     * @brief getShader
     *
     * Gets a shader from storage
     */
    std::shared_ptr<Shader> getShader(const std::string &name);
    std::shared_ptr<Shader> getActiveShader();

    /**
     * @brief addShape
     *
     * Add a shape to storage. Shape can be specified via vertex data or using a Shape
     * object
     */
    void addShape(const std::string &name, const std::vector<float> &data);
    void addShape(const std::string &name, const std::vector<float> &data, VBO::GEOMETRY_LAYOUT layout);
    void addShape(const std::string &name, const std::shared_ptr<Shape> &shape);
    void addShape(const std::string &name, Shape &&shape);

    /**
     * @brief removeShape
     *
     * Remove a shape from storage
     */
    void removeShape(const std::string &name);

    /**
     * @brief getShape
     *
     * Get a shape from storage
     */
    std::shared_ptr<Shape> getShape(const std::string &name);

    /**
     * @brief drawShape
     *
     * Draw the specified shape
     */
    void drawShape(const std::string &name);
    void drawShape(std::shared_ptr<Shape> shape);
    void drawEllipse(glm::vec2 size);
    void drawLine2D(glm::vec2 start, glm::vec2 end, float width);

    /**
     * @brief setUniform
     *
     * Sets a uniform variable on the current shader
     */
    template<class T>
    void setUniform(const std::string &name, const T &v) {
        if(m_activeShader == nullptr) return;

        m_activeShader->setUniform(name, v);

#if GRAPHICS_DEBUG_LEVEL > 0
        m_activeShader->m_trackedUniforms[name] = true;
#endif

    }

    /**
     * @brief setTransform
     *
     * Sets the model transform (matrix) for the current shader
     */
    void setTransform(const glm::mat4 &transform);

    /**
     * @brief clearTransform
     *
     * Sets the model transform to the identity matrix for the current shader
     */
    void clearTransform();

    /**
     * @brief pushTransform
     *
     * Multiplies the current transform by the given transform using the formula
     *
     *          currTransform = currTransform * transform
     *
     * Thus the transformed pushed the latest will be applied first
     */
    void pushTransform(const glm::mat4 &transform);

    /**
     * @brief translate
     *
     * Apply translation to the current transform
     */
    void translate(const glm::vec3 &t);

    /**
     * @brief scale
     *
     * Apply scale to the current transform
     */
    void scale(float s);
    void scale(const glm::vec3 &s);

    /**
     * @brief rotate
     *
     * Apply rotation to the current transform
     */
    void rotate(float angle, const glm::vec3 &a);

    /**
     * @brief getTransform
     *
     * Get the current transform
     */
    glm::mat4 getTransform();

    /**
     * @brief setView
     *
     * Set the view matrix (related to the camera)
     */
    void setView(const glm::mat4 &view);

    /**
     * @brief setProjection
     *
     * Set the projection matrix (related to the camera)
     */
    void setProjection(const glm::mat4 &projection);

    /**
     * @brief setCamera
     *
     * Set the active camera, and set its view and projection matrices
     */
    void setCamera(const std::shared_ptr<Camera> &camera);

    /**
     * @brief getActiveCamera
     *
     * Get the active camera
     */
    std::shared_ptr<Camera> getActiveCamera();

    /**
     * Set various uniforms on the current shader
     */
    void setUseTexture(const int &useTexture);
    void setUseLighting(const int &useLighting);
    void setColor(const glm::vec3 &color);
    void setColor(const glm::vec4 &color);
    void setAlpha(float alpha);
    void setSpecularColor(const glm::vec3 &color);
    void setSpecularColor(const glm::vec4 &color);
    void setShininess(float shininess);
    void setTextureRepeat(const glm::vec2 &repeat);
    void setTextureStart(const glm::vec2 &start);
    void setTextureEnd(const glm::vec2 &end);
    void setIsFont(const int &isFont);
    void setFontTextureStart(const glm::vec2 &start);
    void setFontTextureEnd(const glm::vec2 &end);

    /**
     * @brief addMaterial
     *
     * Adds a material to storage
     */
    void addMaterial(const std::string &name, const Material &m);

    /**
     * @brief removeMaterial
     *
     * Removes a material from storage
     */
    void removeMaterial(const std::string &name);

    /**
     * @brief getMaterial
     *
     * Gets a material from storage
     */
    Material& getMaterial(const std::string &name);
    Material& getActiveMaterial();

    /**
     * @brief setMaterial
     *
     * Sets the current material, to be used by the current shader
     */
    void setMaterial(const Material &m);
    void setMaterial(std::string name);

    /**
     * @brief setDefaultMaterial
     *
     * Sets the current material to the default material
     */
    void setDefaultMaterial();

    /**
     * @brief addTexture
     *
     * Adds a texture to storage. Can be specified by image file, or by
     * Texture2D object
     */
    void addTexture(const std::string &name, const std::string &file);
    void addTexture(const std::string &name, const std::shared_ptr<Texture2D> &tex);
    void addTexture(const std::string &name, Texture2D &&tex);

    /**
     * @brief removeTexture
     *
     * Removes a texture from storage
     */
    void removeTexture(const std::string &name);

    /**
     * @brief setTexture
     *
     * Sets the texture for the current shader
     */
    void setTexture(const std::string &name);
    void setTexture(const std::shared_ptr<Texture2D> &tex);
    void setTexture(const Texture2D &tex);

    /**
     * @brief setFontTexture
     *
     * Sets the font texture for the current shader
     */
    void setFontTexture(const std::string &name);
    void setFontTexture(const std::shared_ptr<Texture2D> &tex);
    void setFontTexture(const Texture2D &tex);

    /**
     * @brief getTexture
     *
     * Gets a texture from storage
     */
    std::shared_ptr<Texture2D> getTexture(const std::string &name);

    /**
     * @brief addFont
     *
     * Adds a font to storage
     */
    void addFont(const std::string &name, const std::string &file);
    void addFont(const std::string &name, const std::shared_ptr<Font> &font);
    void addFont(const std::string &name, Font &&font);

    /**
     * @brief removeFont
     *
     * Removes a font from storage
     */
    void removeFont(const std::string &name);

    /**
     * @brief drawText
     *
     * Draws text using the specified font
     */
    void drawText(const std::string &text, float charSize);
    void drawText(const std::string &name, const std::string &text, float charSize);
    void drawText(const std::string &text);
    void drawText(const std::string &name, const std::string &text);

    /**
     * @brief getFontMetrics
     *
     * Get geometric information about the given text, rendered using the given font
     */
    FontMetrics getFontMetrics(const std::string &name, const std::string &text, float charSize);

    /**
     * @brief getFont
     *
     * Get a font from storage
     */
    std::shared_ptr<Font> getFont(const std::string &name);

    /**
     * @brief addFramebuffer
     *
     * Add a framebuffer to storage
     */
    void addFramebuffer(const std::string &name, const std::shared_ptr<FBO> &fbo);

    /**
     * @brief removeFramebuffer
     *
     * Removes a framebuffer from storage
     */
    void removeFramebuffer(const std::string &name);

    /**
     * @brief setFramebuffer
     *
     * Sets the current framebuffer (everything will be rendered to this framebuffer)
     */
    void setFramebuffer(const std::string &name);

    /**
     * @brief setDefaultFramebuffer
     *
     * Sets the current framebuffer to the default framebuffer (everything will be
     * rendered to the screen)
     */
    void setDefaultFramebuffer();

    /**
     * @brief getFramebuffer
     *
     * Get a framebuffer from storage
     */
    std::shared_ptr<FBO> getFramebuffer(const std::string &name);
    std::shared_ptr<FBO> getActiveFramebuffer();

    /**
     * @brief drawFramebuffer
     *
     * Draw the textures contained by the framebuffers to the screen
     */
    void drawFramebuffer(const std::string &name);
    void drawFramebuffer(const std::shared_ptr<FBO> &fbo);
    void drawFramebuffer(const std::string &name, const glm::vec2 &pos, const glm::vec2 &size);
    void drawFramebuffer(const std::shared_ptr<FBO> &fbo, const glm::vec2 &pos, const glm::vec2 &size);

    /**
     * @brief setLightUniforms
     *
     * Set the uniforms for a given light
     */
    void setLightUniforms(const Light &light, int index);

    /**
     * @brief setLight
     *
     * Set the light at index 'index' to be the given light
     */
    void setLight(const Light &light, int index);

    /**
     * @brief addLight
     *
     * Add the given light to the scene
     */
    void addLight(const Light &light);

    /**
     * @brief clearLights
     *
     * Remove all lights from the scene, restoring default lighting
     */
    void clearLights();

    /**
     * Various graphics helper functions
     */
    void enableBlendTest();
    void enableBlendTest(BLEND_FUNC func);
    void disableBlendTest();
    bool getBlend();
    void enableDepthTest();
    void enableDepthTest(DEPTH_FUNC func);
    void disableDepthTest();
    bool getDepthTest();
    void enableBackfaceCulling();
    void disableBackfaceCulling();
    bool getBackfaceCulling();
    void enableStencilTest(STENCIL_FUNC func, int ref);
    void enableStencilTest(STENCIL_FUNC func, int ref, unsigned int mask);
    void disableStencilTest();
    bool getStencilTest();

    void setClearColor(const glm::vec4 &color);
    void setClearColor(const glm::vec3 &color);
    void clearScreen();
    void clearScreen(CLEAR_FLAG flag);
    void setViewport(const glm::vec2 &start, const glm::vec2 &size);

    /**
     * Debugging helpers
     */
    void printDebug();
    void resetDebug();
    void printFBODebug();
    void resetFBODebug();
    void printShaderDebug();
    void resetShaderDebug();

    /**
     * @brief getGlobalInstance
     *
     * Get the global instance of the graphics object. Can be called from anywhere,
     * always returns the same object
     */
    static Graphics* getGlobalInstance() {
        static std::shared_ptr<Graphics> instance = std::make_shared<Graphics>();
        return instance.get();
    }

    Graphics(Graphics const&) = delete;
    void operator=(Graphics const&) = delete;

private:
    void addQuad();
    void addUIQuad();
    void addCircle();
    void addCylinder();
    void addSphere();
    void addCube();

private:
    // Transform
    glm::mat4 m_transform;

    // Resource management
    std::map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::map<std::string, Material> m_materials;
    std::map<std::string, std::shared_ptr<Shape>> m_shapes;
    std::map<std::string, std::shared_ptr<Texture2D>> m_textures;
    std::map<std::string, std::shared_ptr<Font>> m_fonts;
    std::map<std::string, std::shared_ptr<FBO>> m_framebuffers;

    std::shared_ptr<Shader> m_activeShader;
    std::string m_activeShaderName;

    Material m_activeMaterial;
    std::string m_activeMaterialName;

    std::shared_ptr<FBO> m_activeFBO;
    std::string m_activeFBOName;

    std::shared_ptr<Camera> m_activeCamera;

    // Lighting
    int m_numLights;
    std::vector<Light> m_lights;

    // Other
    bool m_blend = false;
    bool m_depthTest = false;
    bool m_culling = false;
    bool m_stencil = false;

    // Debug variables
    std::map<std::string, bool> m_trackedProperties;
    std::map<std::string, bool> m_trackedFBOProperties;
    bool m_screenCleared = false;
};

#endif // GRAPHICS_H
