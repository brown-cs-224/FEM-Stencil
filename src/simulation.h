#ifndef SIMULATION_H
#define SIMULATION_H

#include "graphics/shape.h"

class Shader;

class Simulation
{
public:
    Simulation();

    void init();

    void update(float seconds);

    void draw(Shader *shader);

    void toggleWire();
private:
    Shape m_shape;

    Shape m_ground;
    void initGround();
};

#endif // SIMULATION_H
