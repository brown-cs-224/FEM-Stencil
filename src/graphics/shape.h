#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <vector>

#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#include <Eigen/StdVector>
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix3i)
#include <Eigen/Dense>

class Shader;

class Shape
{
public:
    Shape();

    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals, const std::vector<Eigen::Vector3i> &triangles);
    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles);
    void init(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles, const std::vector<Eigen::Vector4i> &tetIndices);

    void setVertices(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3f> &normals);
    void setVertices(const std::vector<Eigen::Vector3f> &vertices);

    void setModelMatrix(const Eigen::Affine3f &model);

    void toggleWireframe();

    void draw(Shader *shader);

private:
    GLuint m_surfaceVao;
    GLuint m_tetVao;
    GLuint m_surfaceVbo;
    GLuint m_tetVbo;
    GLuint m_surfaceIbo;
    GLuint m_tetIbo;

    unsigned int m_numSurfaceVertices;
    unsigned int m_numTetVertices;
    unsigned int m_verticesSize;
    float m_red;
    float m_blue;
    float m_green;
    float m_alpha;

    std::vector<Eigen::Vector3i> m_faces;

    Eigen::Matrix4f m_modelMatrix;

    bool m_wireframe;
};

#endif // SHAPE_H
