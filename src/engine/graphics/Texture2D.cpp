#include "Texture2D.h"

#include <utility>

Texture2D::Texture2D(unsigned char *data, int width, int height, Texture::DATA_TYPE type, int numComponents)
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
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat , width, height, 0, format, gltype, data);
    unbind();
}

void Texture2D::_setFilterMethod(GLenum filter) {
    this->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    this->unbind();
}

void Texture2D::_setWrapMethod(GLenum wrap) {
    this->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    this->unbind();
}

void Texture2D::_setTextureParams(GLenum filter, GLenum wrap) {
    this->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    this->unbind();
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
