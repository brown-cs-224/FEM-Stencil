#include "Texture1D.h"

#include <utility>

Texture1D::Texture1D(unsigned char *data, int width, Texture::DATA_TYPE type, int numComponents)
{
    GLenum format, internalFormat;

    switch(numComponents) {
    case 1:
        format = GL_RED;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        format = GL_RGBA;
        break;
    }

    GLenum gltype = (GLenum) type;
    switch(gltype) {
    case GL_FLOAT:
        if(numComponents == 1)
            internalFormat = GL_R32F;
        else if(numComponents == 2)
            internalFormat = GL_RG32F;
        else if(numComponents == 3)
            internalFormat = GL_RGB32F;
        else if(numComponents == 4)
            internalFormat = GL_RGBA32F;
        break;
    case GL_INT:
        if(numComponents == 1) {
            internalFormat = GL_R32I;
            format = GL_RED_INTEGER;
        }
        else if(numComponents == 2) {
            internalFormat = GL_RG32I;
            format = GL_RG_INTEGER;
        }
        else if(numComponents == 3) {
            internalFormat = GL_RGB32I;
            format = GL_RGB_INTEGER;
        }
        else if(numComponents == 4) {
            internalFormat = GL_RGBA32I;
            format = GL_RGBA_INTEGER;
        }
        break;
    case GL_UNSIGNED_BYTE:
        internalFormat = format;
        break;
    default:
        internalFormat = format;
        break;
    }

    // Bind the texture by calling bind() and filling it in
    // Generate the texture with glTexImage2D
    Texture::setTextureParams(Texture::FILTER_METHOD::LINEAR, Texture::WRAP_METHOD::CLAMP_TO_EDGE);

    bind();
    glTexImage1D(GL_TEXTURE_1D, 0, internalFormat , width, 0, format, gltype, data);
    unbind();
}

void Texture1D::_setFilterMethod(GLenum filter) {
    this->bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
    this->unbind();
}

void Texture1D::_setWrapMethod(GLenum wrap) {
    this->bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
    this->unbind();
}

void Texture1D::_setTextureParams(GLenum filter, GLenum wrap) {
    this->bind();
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap);
    this->unbind();
}

void Texture1D::bind() const {
    glBindTexture(GL_TEXTURE_1D, m_handle);
}

void Texture1D::unbind() const {
    glBindTexture(GL_TEXTURE_1D, 0);
}
