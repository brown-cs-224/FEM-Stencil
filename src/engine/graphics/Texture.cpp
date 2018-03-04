#include "Texture.h"

#include <cassert>
#include <utility>

#include <GL/glew.h>

Texture::Texture() :
    m_handle(0)
{
    glGenTextures(1, &m_handle);
}

Texture::Texture(Texture &&that) :
    m_handle(that.m_handle)
{
    that.m_handle = 0;
}

Texture& Texture::operator=(Texture &&that) {
    this->~Texture();
    m_handle = that.m_handle;
    that.m_handle = 0;
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

unsigned int Texture::id() const {
    return m_handle;
}

void Texture::setFilterMethod(FILTER_METHOD filter) {
    this->_setFilterMethod((GLenum) filter);
}

void Texture::setWrapMethod(WRAP_METHOD wrap) {
    this->_setWrapMethod((GLenum) wrap);
}

void Texture::setTextureParams(FILTER_METHOD filter, WRAP_METHOD wrap) {
    this->_setTextureParams((GLenum) filter, (GLenum) wrap);
}
