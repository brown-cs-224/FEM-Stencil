#include "Light.h"

#include "engine/graphics/Constants.h"

#include <limits>

Light::Light(LIGHT_TYPE type,
             glm::vec3 color,
             glm::vec3 dir,
             glm::vec3 pos,
             glm::vec2 att) :
    type(type), color(color), dir(dir), pos(pos), att(att)
{
}

Light::Light(const Light &that) :
    type(that.type),
    color(that.color),
    dir(that.dir),
    pos(that.pos),
    att(that.att)
{
}

Light& Light::operator=(const Light &that) {
    type = that.type;
    color = that.color;
    dir = that.dir;
    pos = that.pos;
    att = that.att;

    return *this;
}

float Light::getRadius() const {
    // Solve th * (att.y * d^2 + att.x * d + 1) - 1 = 0 for d
    // This is where the total power of the light is th * original power
    if(type == LIGHT_TYPE::POINT) {
        float th = GraphicsConstants::LIGHT_THRESHOLD;

        // Discriminant
        float disc = (th * th * att.x * att.x) - 4 * (th * att.y) * (th - 1);

        // Solve
        if(disc < 0) {
            return -1;
        }
        else {
            return (-th * att.x + glm::sqrt(disc)) / (2 * (th * att.y));
        }
    }
    else {
        return -1;
    }
}
