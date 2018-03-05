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
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                                "C:/Users/Graphics Lab/Documents/Tyler/cs224-pathtracing-stencil/path-stencil/example-scenes/models/Sphere.obj", nullptr, true);
    if(!ret) {
        std::cout << err << std::endl;
        return;
    }
    std::vector<Vector3i> faces;
    for(size_t s = 0; s < shapes.size(); ++s) {
        size_t indexOffset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            Vector3i face;
            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                face[v] = idx.vertex_index;
            }
            faces.push_back(face);

            indexOffset += fv;
        }
    }
    for(size_t i = 0; i < attrib.vertices.size(); i += 3) {
        m_vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }
    m_shape.init(m_vertices, faces);
}

void Simulation::update(float seconds)
{

}

void Simulation::draw()
{
    m_shape.draw();
}
