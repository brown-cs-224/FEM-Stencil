#ifndef SIMULATION_H
#define SIMULATION_H

#include "graphics/shape.h"

class Simulation
{
public:
    Simulation();

    void init();

    void update(float seconds);

    void draw();

private:
    std::vector<Eigen::Vector3f> m_vertices;
    Shape m_shape;
};

#endif // SIMULATION_H
