#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <vector>

#include <Eigen/Dense>

class Shader;

class Shape
{
public:
    Shape();

    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals, const std::vector<Eigen::Vector3i> &triangles);
    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles);

    void setVertices(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals);
    void setVertices(const std::vector<Eigen::Vector3f> &vertices);

    void setModelMatrix(const Eigen::Affine3f &model);

    void draw(Shader *shader);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    unsigned int m_numVertices;
    unsigned int m_verticesSize;
    std::vector<Eigen::Vector3i> m_faces;

    Eigen::Matrix4f m_modelMatrix;
};

#endif // SHAPE_H
