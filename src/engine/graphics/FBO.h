#ifndef FBO_H
#define FBO_H

#include <memory>
#include <vector>

#include "Texture.h"

class RenderBuffer;
class Texture2D;

class FBO {
public:

    enum class DEPTH_STENCIL_ATTACHMENT { DEPTH_ONLY, DEPTH_STENCIL, NONE };

    FBO(int width, int height, int numberOfColorAttachments = 1, int numColorComponents = 4, DEPTH_STENCIL_ATTACHMENT attachmentType = DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY,
        Texture::FILTER_METHOD filterMethod = Texture::FILTER_METHOD::LINEAR,
        Texture::WRAP_METHOD wrapMethod = Texture::WRAP_METHOD::REPEAT,
        Texture::DATA_TYPE type = Texture::DATA_TYPE::UNSIGNED_BYTE);
    ~FBO();

    void bind();
    void unbind();
    unsigned int id() const { return m_handle; }
    int width() const { return m_width; }
    int height() const { return m_height; }

    const Texture2D& getColorAttachment(int i) const;
    const RenderBuffer& getDepthStencilAttachment() const;
    size_t getNumColorAttachments() const;

    void copyDepthFrom(const FBO &other);
    void copyDepthStencilFrom(const FBO &other);

private:
    void generateColorAttachments(int count, int numComponents, Texture::FILTER_METHOD filterMethod,
                                  Texture::WRAP_METHOD wrapMethod, Texture::DATA_TYPE type);
    void generateColorAttachment(int i, int numComponents, Texture::FILTER_METHOD filterMethod,
                                 Texture::WRAP_METHOD wrapMethod, Texture::DATA_TYPE type);
    void generateDepthStencilAttachment();

    unsigned int m_handle;
    int m_width;
    int m_height;

    std::vector<Texture2D> m_colorAttachments;
    std::unique_ptr<RenderBuffer> m_depthAttachment;
    DEPTH_STENCIL_ATTACHMENT m_depthStencilAttachmentType;
};

#endif // FBO_H
