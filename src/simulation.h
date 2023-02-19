#pragma once

#include "graphics/shape.h"

class Shader;

class Simulation
{
public:
    Simulation();

    void init();

    void update(double seconds);

    void draw(Shader *shader);

    void toggleWire();
private:
    Shape m_shape;

    Shape m_ground;
    void initGround();
};
