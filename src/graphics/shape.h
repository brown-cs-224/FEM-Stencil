#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <vector>

#include <Eigen/Dense>

class Shape
{
public:
    Shape();

    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals, const std::vector<Eigen::Vector3i> &triangles);
    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles);

    void setVertices(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals);
    void setVertices(const std::vector<Eigen::Vector3f> &vertices);

    void draw();

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    unsigned int m_numVertices;
    unsigned int m_verticesSize;
    std::vector<Eigen::Vector3i> m_faces;
};

#endif // SHAPE_H
