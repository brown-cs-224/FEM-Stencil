#ifndef FONT_H
#define FONT_H

#include "engine/util/CommonIncludes.h"

#include <string>
#include <memory>

// Forward declarations of structs needed for font rendering
struct AlignedQuad;
struct PackedChar;
struct FontInfo;


// Other forward declarations
class Graphics;
class Texture2D;

/**
 * @brief FontMetrics
 *
 * Struct containing info about text size
 */
struct FontMetrics {
    FontMetrics(float a, float d, float w) : ascent(a), descent(d), width(w) {}

    float ascent; // units that text ascends above the position where it is drawn
    float descent; // units that text descends below the position where it is drawn
    float width; // width of text
};

/**
 * @brief Font
 * @param path - path to the .ttf file that defines the font
 * @param fontResolution - resolution of the font (how many pixels tall a character is in the font texture). Higher
 *      resolution increases sharpness of characters, but does not affect size upon rendering text
 * @param fontTextureDimensions - dimensions of the font texture, might need to be larger if fontResolution is larger
 * @param oversampling - amount of antialasing used for the font texture
 * @param charOffset - unicode value of the first character this font stores
 * @param numChars - how many characters this font stores
 *
 * Knows how to draw text using a specific font. Also supports querying information about text,
 * drawn at a certain point size using the same font.
 */
class Font
{
public:
    /**
     * @brief Font
     * @param path - path to the .ttf file that defines the font
     * @param fontResolution - resolution of the font (how many pixels tall a character is in the font texture). Higher
     *      resolution increases sharpness of characters, but does not affect size upon rendering text
     * @param fontTextureDimensions - dimensions of the font texture, might need to be larger if fontResolution is larger
     * @param oversampling - amount of antialasing used for the font texture
     * @param charOffset - unicode value of the first character this font stores
     * @param numChars - how many characters this font stores
     *
     * Creates a font class using the given parameters
     */
    Font(const std::string &path, float fontResolution = 50, glm::vec2 fontTextureDimensions = glm::vec2(1024, 1024),
         int oversampling = 2,
         int charOffset = 32, int numChars = 96);
    virtual ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font &&that);
    Font& operator=(Font && that);

    /**
     * @brief Font::drawText
     *
     * @param g - graphics  object
     * @param text - text to render
     * @param charSize - size of a character in model coordinates
     *
     * Renders text using the given shader, with the specified point size and start position
     */
    void drawText(Graphics *g, const std::string &text, float charSize);
    void drawText(const std::shared_ptr<Graphics> &g, const std::string &text, float charSize);

    /**
     * @brief Font::getFontMetrics
     * @param charSize - size of a character in model coordinates
     * @param text - text to render
     *
     * @return FontMetrics object, containing information about the ascent, descent, and width of the text given its charSize
     */
    FontMetrics getFontMetrics(const std::string &text, float charSize);

private:
    /**
     * @brief Font::_loadFont
     * @param path
     *
     * Packs font bitmap data into m_fontTex, and stores information about character positioning (within the texture)
     * in m_charData.
     *
     */
    void _loadFont(const std::string &path);

    /**
     * @brief Font::_drawQuad
     *
     * Helper method to draw quad with the given start position, end position, and uv coordinates
     */
    void _drawQuad(Graphics *g, float x0, float y0, float x1, float y1, float s0, float t0, float s1, float t1);

private:
    float m_fontRes;
    std::shared_ptr<Texture2D> m_fontTex;
    glm::vec2 m_fontTexDims;
    int m_oversampling;

    int m_charOffset, m_numChars;
    std::unique_ptr<PackedChar[]> m_charData;
};

#endif // FONT_H
