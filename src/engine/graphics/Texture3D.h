#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "Texture.h"

#include "GL/glew.h"

/**
 * @class Texture3D
 *
 * Wrapper class for OpenGL 3D Textures
 */
class Texture3D : public Texture {
public:
    Texture3D(unsigned char *data, int width, int height, int depth, Texture::DATA_TYPE type = Texture::DATA_TYPE::UNSIGNED_BYTE, int numComponents = 4);

    virtual void bind() const override;
    virtual void unbind() const override;

protected:
    void _setFilterMethod(GLenum filter) override;
    void _setWrapMethod(GLenum wrap) override;
    void _setTextureParams(GLenum filter, GLenum wrap) override;
};

#endif // TEXTURE3D_H
