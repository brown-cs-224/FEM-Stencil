#include "DepthBuffer.h"

#include "GL/glew.h"

DepthBuffer::DepthBuffer(int width, int height, bool hasStencil) :
    m_width(width),
    m_height(height),
    m_stencil(hasStencil)
{
    bind();
    if(!hasStencil) {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    }
    else {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    unbind();
}
