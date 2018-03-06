#include "simulation.h"

#include <iostream>

#include "graphics/MeshLoader.h"

using namespace Eigen;

Simulation::Simulation()
{
}

void Simulation::init()
{
    //TODO students: replace this example code with loading a mesh and extracting the surface
    std::vector<Vector3f> vertices;
    std::vector<Vector4i> tets;
    if(MeshLoader::loadTetMesh("example-meshes/single-tet.mesh", vertices, tets)) {
        std::vector<Vector3i> faces;
        faces.emplace_back(0, 1, 2);
        faces.emplace_back(0, 2, 3);
        faces.emplace_back(1, 3, 2);
        faces.emplace_back(0, 3, 1);
        m_shape.init(vertices, faces, tets);
    }
    m_shape.setModelMatrix(Affine3f(Eigen::Translation3f(0, 2, 0)));

    initGround();
}

void Simulation::update(float seconds)
{
    //TODO students: calculate the vertices and the surface and call m_shape.setVertices with the new vertices
}

void Simulation::draw(Shader *shader)
{
    m_shape.draw(shader);
    m_ground.draw(shader);
}

void Simulation::toggleWire()
{
    m_shape.toggleWireframe();
}

void Simulation::initGround()
{
    std::vector<Vector3f> groundVerts;
    std::vector<Vector3i> groundFaces;
    groundVerts.emplace_back(-5, 0, -5);
    groundVerts.emplace_back(-5, 0, 5);
    groundVerts.emplace_back(5, 0, 5);
    groundVerts.emplace_back(5, 0, -5);
    groundFaces.emplace_back(0, 1, 2);
    groundFaces.emplace_back(0, 2, 3);
    m_ground.init(groundVerts, groundFaces);
}
