#ifndef SHAPE_H
#define SHAPE_H

#include "engine/util/CommonIncludes.h"
#include "VBO.h"

#include <vector>
#include <memory>

class Graphics;

/**
 * @brief The Shape class
 *
 * Creates a shape with the given shape data
 */
class Shape
{
public:
    Shape(VBO::GEOMETRY_LAYOUT layout=VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);

    Shape(const std::vector<float> &vertices,
          VBO::GEOMETRY_LAYOUT layout=VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);

    Shape(const std::vector<float> &vertices,
          const std::vector<int> &faces,
          VBO::GEOMETRY_LAYOUT layout=VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);

    Shape(const std::vector<float> &positions,
          const std::vector<float> &normals,
          const std::vector<float> &texCoords,
          VBO::GEOMETRY_LAYOUT layout=VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);

    Shape(const std::vector<float> &positions,
          const std::vector<float> &normals,
          const std::vector<float> &texCoords,
          const std::vector<int> &faces,
          VBO::GEOMETRY_LAYOUT layout=VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES);

    Shape(std::string shape);
    virtual ~Shape();

    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;
    Shape(Shape &&that);
    Shape& operator=(Shape && that);

    void addPosition(float x, float y, float z);
    void addPositions(const std::vector<float> &positions);

    void addNormal(float x, float y, float z);
    void addNormals(const std::vector<float> &normals);

    void addTextureCoordinate(float s, float t);
    void addTextureCoordinates(const std::vector<float> &texCoords);

    void addVertex(float x, float y, float z, float n1, float n2, float n3, float s, float t);
    void addVertices(const std::vector<float> &vertices);

    void addFace(int v1, int v2, int v3);
    void addFaces(const std::vector<int> &faces);

    void setShapeName(std::string shape);
    std::string getShapeName();

    void setPos(glm::vec3 pos);
    void translate(glm::vec3 delta);
    glm::vec3 getPos();
    void setScale(glm::vec3 scale);
    void setScale(float scale);
    void scale(glm::vec3 scale);
    void scale(float scale);
    glm::vec3 getScale();
    void setRotation(float angle, glm::vec3 axis);
    void setRotation(glm::quat r);
    void setAngle(float angle);
    void setAxis(glm::vec3 axis);
    void rotate(float angle, glm::vec3 axis);
    void rotate(glm::quat r);
    void rotate(float angle);
    float getAngle();
    glm::vec3 getAxis();
    glm::quat getRotation();
    glm::mat4 getModelMatrix();

    void draw(const std::shared_ptr<Graphics> &g);
    void draw(Graphics *g);

    bool printDebug();

private:
    void build();

private:
    std::vector<float> m_positions;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;
    std::vector<int> m_faces;

    VBO::GEOMETRY_LAYOUT m_vboTriangleLayout;
    GLenum m_triangleLayout;
    GLuint m_numVertices;
    GLuint m_handle;

    bool m_indexed;
    bool m_built;

    friend class Graphics;

    std::string m_shapeName;
    glm::vec3 m_pos;
    glm::vec3 m_scale;
    glm::quat m_rotation;
};

#endif // SHAPE_H
