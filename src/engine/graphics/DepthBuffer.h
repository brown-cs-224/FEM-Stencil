#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "RenderBuffer.h"

class DepthBuffer : public RenderBuffer {
public:
    DepthBuffer(int width, int height, bool hasStencil);

    bool hasStencil() { return m_stencil; }

private:
    int m_width;
    int m_height;

    bool m_stencil;
};

#endif // DEPTHBUFFER_H
