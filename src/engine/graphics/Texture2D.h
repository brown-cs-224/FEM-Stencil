#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"

#include "GL/glew.h"

/**
 * @class Texture2D
 *
 * Wrapper class for OpenGL 2D Textures. This is probably the class you'll want to use
 * for all of your texturing purposes.
 */
class Texture2D : public Texture {
public:
    Texture2D(unsigned char *data, int width, int height, Texture::DATA_TYPE type = Texture::DATA_TYPE::UNSIGNED_BYTE, int numComponents = 4);

    virtual void bind() const override;
    virtual void unbind() const override;

protected:
    void _setFilterMethod(GLenum filter) override;
    void _setWrapMethod(GLenum wrap) override;
    void _setTextureParams(GLenum filter, GLenum wrap) override;
};

#endif // TEXTURE2D_H
