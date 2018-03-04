#ifndef TEXTURE_H
#define TEXTURE_H

#include "engine/util/CommonIncludes.h"

/**
 * @class Texture
 *
 * Abstract texture class
 */
class Texture {
public:
    enum class FILTER_METHOD { NEAREST = GL_NEAREST,
                               LINEAR = GL_LINEAR };

    enum class WRAP_METHOD { CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
                             REPEAT = GL_REPEAT };

    enum class DATA_TYPE { FLOAT = GL_FLOAT,
                      INT = GL_INT,
                      UNSIGNED_BYTE = GL_UNSIGNED_BYTE };
public:
    Texture();
    virtual ~Texture();
    Texture(const Texture &that) = delete;
    Texture& operator=(const Texture &that) = delete;
    Texture(Texture &&that);
    Texture& operator=(Texture &&that);

    virtual void bind() const = 0;
    unsigned int id() const;
    virtual void unbind() const = 0;

    void setFilterMethod(FILTER_METHOD filter);
    void setWrapMethod(WRAP_METHOD wrap);
    void setTextureParams(FILTER_METHOD filter, WRAP_METHOD wrap);

protected:
    virtual void _setFilterMethod(GLenum filter) = 0;
    virtual void _setWrapMethod(GLenum wrap) = 0;
    virtual void _setTextureParams(GLenum filter, GLenum wrap) = 0;

protected:
    unsigned int m_handle;
};

#endif // TEXTURE_H
