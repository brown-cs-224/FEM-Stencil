#include "simulation.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "libs/tiny_obj/tiny_obj_loader.h"

#include <iostream>

using namespace Eigen;

Simulation::Simulation()
{
}

void Simulation::init()
{
}

void Simulation::update(float seconds)
{
}

void Simulation::draw()
{
    m_shape.draw();
}
