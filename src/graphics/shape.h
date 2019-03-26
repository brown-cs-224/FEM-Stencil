#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>

class Shader;

class Shape
{
public:
    Shape();

    void init(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::ivec3> &triangles);
    void init(const std::vector<glm::vec3> &vertices, const std::vector<glm::ivec3> &triangles);
    void init(const std::vector<glm::vec3> &vertices, const std::vector<glm::ivec3> &triangles, const std::vector<glm::ivec4> &tetIndices);

    void setVertices(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals);
    void setVertices(const std::vector<glm::vec3> &vertices);

    void setModelMatrix(const glm::mat4x4 &model);
    glm::mat4x4 getModelMatrix();
    glm::mat4x4 getInverseModelMatrix();

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
    std::vector<glm::ivec3> m_faces;

    glm::mat4x4 m_modelMatrix, m_invModelMatrix;

    bool m_wireframe;
};

#endif // SHAPE_H
