#include "Font.h"

#include <QFile>
#include <QTextStream>
#include <vector>
#include <iostream>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"

#include "engine/graphics/Graphics.h"
#include "engine/graphics/Texture2D.h"

struct AlignedQuad : public stbtt_aligned_quad {};
struct PackedChar : public stbtt_packedchar {};
struct FontInfo : public stbtt_fontinfo {};

/**
 * @brief readFontFile
 * @param path -- path to .ttf file
 *
 * @return raw data in the font file
 */
std::vector<uint8_t> readFontFile(const std::string &path)
{
    // Create and open file
    QString fontFilePath = QString::fromStdString(path);
    QFile fontFile(fontFilePath);

    if(!fontFile.open(QIODevice::ReadOnly)) {
        std::cerr << "Could not open font file" << std::endl;
        std::exit(-1);
    }

    // Get size
    long size = fontFile.size();

    // Read font data
    fontFile.seek(0);
    std::vector<uint8_t> fontData = std::vector<uint8_t>(size);
    fontFile.read(reinterpret_cast<char*>(&fontData[0]), size);

    // Finish
    fontFile.close();
    return fontData;
}

Font::Font(const std::string &path, float fontResolution, glm::vec2 fontTextureDimensions, int oversampling, int charOffset, int numChars) :
    m_fontRes(fontResolution), m_fontTex(nullptr), m_fontTexDims(fontTextureDimensions), m_oversampling(oversampling), m_charOffset(charOffset), m_numChars(numChars)
{
    _loadFont(path);
}

Font::~Font()
{
}

Font::Font(Font &&that) :
    m_fontRes(that.m_fontRes),
    m_fontTex(std::move(that.m_fontTex)),
    m_fontTexDims(that.m_fontTexDims),
    m_oversampling(that.m_oversampling),
    m_charOffset(that.m_charOffset),
    m_numChars(that.m_numChars),
    m_charData(std::move(that.m_charData))
{

}

Font& Font::operator=(Font && that) {
    m_fontRes = that.m_fontRes;
    m_fontTex = std::move(that.m_fontTex);
    m_fontTexDims = that.m_fontTexDims;
    m_oversampling = that.m_oversampling;
    m_charOffset = that.m_charOffset;
    m_numChars = that.m_numChars;
    m_charData = std::move(that.m_charData);

    return *this;
}

void Font::_loadFont(const std::string &path)
{
    // Character positioning and font metadata
    m_charData = std::make_unique<PackedChar[]>(m_numChars);

    // Bitmap in which to store font
    std::unique_ptr<uint8_t[]> fontBitmap = std::make_unique<uint8_t[]>(m_fontTexDims.x * m_fontTexDims.y);

    // Read font data
    std::vector<uint8_t> fontData = readFontFile(path);

    // Initialize pack context
    stbtt_pack_context packContext;
    if(!stbtt_PackBegin(&packContext, fontBitmap.get(), m_fontTexDims.x, m_fontTexDims.y, 0, 1, nullptr)) {
        std::cerr << "Failed to load font" << std::endl;
        std::exit(-1);
    }

    // Pack font into fontBitmap, and put character position metadata in m_charData
    stbtt_PackSetOversampling(&packContext, m_oversampling, m_oversampling);
    stbtt_PackFontRange(&packContext, fontData.data(), 0, m_fontRes, m_charOffset, m_numChars, m_charData.get());
    stbtt_PackEnd(&packContext);

    // Create font texture from fontBitmap
    m_fontTex = std::make_shared<Texture2D>(
                fontBitmap.get(),
                m_fontTexDims.x,
                m_fontTexDims.y,
                Texture::DATA_TYPE::UNSIGNED_BYTE,
                1
                );
    m_fontTex->setWrapMethod(Texture::WRAP_METHOD::CLAMP_TO_EDGE);
}

void Font::drawText(const std::shared_ptr<Graphics> &g, const std::string &text, float charSize) {
    drawText(g.get(), text, charSize);
}

void Font::drawText(Graphics *g, const std::string &text, float charSize)
{
    // Initialize quad
    float charX = 0, charY = 0;
    AlignedQuad quad;

    // Bind texture
    g->setIsFont(1);
    g->setFontTexture(m_fontTex);

    // Transform
    glm::mat4 lastTransform = g->getTransform();
    g->scale(charSize / m_fontRes);

    for(uint i = 0; i < text.length(); i++) {
        char c = text[i];

        // Get quad data
        stbtt_GetPackedQuad(m_charData.get(), m_fontTexDims.x, m_fontTexDims.y, c - m_charOffset, &charX, &charY, &quad, 0);

        // Draw quad
        _drawQuad(g, quad.x0, -quad.y1, quad.x1, -quad.y0, quad.s0, quad.t1, quad.s1, quad.t0);
    }

    // Reset uniforms
    g->setIsFont(0);
    g->setTransform(lastTransform);
}

void Font::_drawQuad(Graphics *g, float x0, float y0, float x1, float y1, float s0, float t0, float s1, float t1)
{
    // Texture coordinates
    g->setFontTextureStart(glm::vec2(s0, t0));
    g->setFontTextureEnd(glm::vec2(s1, t1));

    // Transform
    glm::mat4 lastTransform = g->getTransform();
    g->translate(glm::vec3(x0, y0, 0));
    g->scale(glm::vec3(x1 - x0, y1 - y0, 1));

    // Draw
    g->drawShape("uiquad");

    // Reset transform
    g->setTransform(lastTransform);
}

FontMetrics Font::getFontMetrics(const std::string &text, float charSize)
{
    // Initialize quad
    float charX = 0, charY = 0;
    AlignedQuad quad;

    // Initial values
    float ascent = -INFINITY;
    float descent = INFINITY;
    float startOffset = INFINITY;

    for(uint i = 0; i < text.length(); i++) {
        char c = text[i];

        // Get quad data
        stbtt_GetPackedQuad(m_charData.get(), m_fontTexDims.x, m_fontTexDims.y, c - m_charOffset, &charX, &charY, &quad, 0);

        // Update font metrics
        ascent = glm::max(-quad.y0, ascent);
        descent = glm::min(-quad.y1, descent);
        startOffset = glm::min(quad.x0, startOffset); // Will be set to the start position of the first quad
    }

    // Width and scale
    float width = quad.x1 + startOffset;
    float scale = charSize / m_fontRes;

    return FontMetrics(ascent * scale, descent * scale, width * scale);
}
