#include "shape.h"

#include <iostream>

#include "graphics/Shader.h"

Shape::Shape()
    : m_numVertices(),
      m_verticesSize(),
      m_modelMatrix(Eigen::Matrix4f::Identity())
{
}

void Shape::init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals, const std::vector<Eigen::Vector3i> &triangles)
{
    if(vertices.size() != normals.size()) {
        std::cerr << "Vertices and normals are not the same size" << std::endl;
        return;
    }
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);
    glGenVertexArrays(1, &m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3 * 2, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size() * 3, static_cast<const void *>(vertices.data()));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, sizeof(float) * vertices.size() * 3, static_cast<const void *>(normals.data()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * triangles.size(), static_cast<const void *>(triangles.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(float) * vertices.size() * 3));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_numVertices = triangles.size() * 3;
    m_verticesSize = vertices.size();
    m_faces = triangles;
}

void Shape::init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles)
{
    std::vector<Eigen::Vector3f> verts;
    std::vector<Eigen::Vector3f> normals;
    std::vector<Eigen::Vector3i> faces;
    verts.reserve(triangles.size() * 3);
    normals.reserve(triangles.size() * 3);
    for(auto& f : triangles) {
        auto& v1 = vertices[f[0]];
        auto& v2 = vertices[f[1]];
        auto& v3 = vertices[f[2]];
        auto& e1 = v2 - v1;
        auto& e2 = v3 - v1;
        auto n = e1.cross(e2);
        int s = verts.size();
        faces.push_back(Eigen::Vector3i(s, s + 1, s + 2));
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
        verts.push_back(v1);
        verts.push_back(v2);
        verts.push_back(v3);
    }
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);
    glGenVertexArrays(1, &m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * 3 * 2, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verts.size() * 3, static_cast<const void *>(verts.data()));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * 3, sizeof(float) * verts.size() * 3, static_cast<const void *>(normals.data()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3 * faces.size(), static_cast<const void *>(faces.data()), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(sizeof(float) * verts.size() * 3));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_numVertices = faces.size() * 3;
    m_verticesSize = vertices.size();
    m_faces = triangles;
}

void Shape::setVertices(const std::vector<Eigen::Vector3f> &vertices)
{
    if(vertices.size() != m_verticesSize) {
        std::cerr << "You can't set vertices to a vector that is a different length that what shape was inited with" << std::endl;
        return;
    }
    std::vector<Eigen::Vector3f> verts;
    std::vector<Eigen::Vector3f> normals;
    verts.reserve(m_faces.size() * 3);
    normals.reserve(m_faces.size() * 3);
    for(auto& f : m_faces) {
        auto& v1 = vertices[f[0]];
        auto& v2 = vertices[f[1]];
        auto& v3 = vertices[f[2]];
        auto& e1 = v2 - v1;
        auto& e2 = v3 - v1;
        auto n = e1.cross(e2);
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
        verts.push_back(v1);
        verts.push_back(v2);
        verts.push_back(v3);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verts.size() * 3, static_cast<const void *>(verts.data()));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * verts.size() * 3, sizeof(float) * verts.size() * 3, static_cast<const void *>(normals.data()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shape::setModelMatrix(const Eigen::Affine3f &model)
{
    m_modelMatrix = model.matrix();
}

void Shape::setVertices(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals)
{
    if(vertices.size() != normals.size()) {
        std::cerr << "Vertices and normals are not the same size" << std::endl;
        return;
    }
    if(vertices.size() != m_verticesSize) {
        std::cerr << "You can't set vertices to a vector that is a different length that what shape was inited with" << std::endl;
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size() * 3, static_cast<const void *>(vertices.data()));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, sizeof(float) * vertices.size() * 3, static_cast<const void *>(normals.data()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shape::draw(Shader *shader)
{
    shader->setUniform("m", m_modelMatrix);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_numVertices, GL_UNSIGNED_INT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}
