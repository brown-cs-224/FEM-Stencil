#include "Shape.h"

#include "engine/graphics/IBO.h"
#include "engine/graphics/VBOAttribMarker.h"
#include "engine/graphics/ShaderAttribLocations.h"
#include "engine/graphics/Graphics.h"

#include <cstdarg>
#include <iostream>

Shape::Shape(VBO::GEOMETRY_LAYOUT layout) :
    m_vboTriangleLayout(layout),
    m_numVertices(0),
    m_indexed(false),
    m_built(false),
    m_shapeName(""),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
}

Shape::Shape(const std::vector<float> &vertices,
             VBO::GEOMETRY_LAYOUT layout) :
    m_vboTriangleLayout(layout),
    m_numVertices(0),
    m_indexed(false),
    m_built(false),
    m_shapeName(""),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
    addVertices(vertices);
}

Shape::Shape(const std::vector<float> &vertices,
             const std::vector<int> &faces,
             VBO::GEOMETRY_LAYOUT layout) :
    m_vboTriangleLayout(layout),
    m_numVertices(0),
    m_indexed(true),
    m_built(false),
    m_shapeName(""),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
    addVertices(vertices);
    addFaces(faces);
}

Shape::Shape(const std::vector<float> &positions,
             const std::vector<float> &normals,
             const std::vector<float> &texCoords,
             VBO::GEOMETRY_LAYOUT layout) :
    m_vboTriangleLayout(layout),
    m_numVertices(0),
    m_indexed(false),
    m_built(false),
    m_shapeName(""),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
    addPositions(positions);
    addNormals(normals);
    addTextureCoordinates(texCoords);
}

Shape::Shape(const std::vector<float> &positions,
             const std::vector<float> &normals,
             const std::vector<float> &texCoords,
             const std::vector<int> &faces,
             VBO::GEOMETRY_LAYOUT layout) :
    m_vboTriangleLayout(layout),
    m_numVertices(0),
    m_indexed(true),
    m_built(false),
    m_shapeName(""),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
    addPositions(positions);
    addNormals(normals);
    addTextureCoordinates(texCoords);
    addFaces(faces);
}

Shape::Shape(std::string shape) :
    m_built(true),
    m_shapeName(shape),
    m_pos(0),
    m_scale(1),
    m_rotation()
{
}

Shape::~Shape() {
    glDeleteVertexArrays(1, &m_handle);
}

Shape::Shape(Shape &&that)  :
    m_positions(that.m_positions),
    m_normals(that.m_normals),
    m_texCoords(that.m_texCoords),
    m_faces(that.m_faces),
    m_vboTriangleLayout(that.m_vboTriangleLayout),
    m_triangleLayout(that.m_triangleLayout),
    m_numVertices(that.m_numVertices),
    m_handle(that.m_handle),
    m_indexed(that.m_indexed),
    m_built(that.m_built),
    m_shapeName(that.m_shapeName),
    m_pos(that.m_pos),
    m_scale(that.m_scale),
    m_rotation(that.m_rotation)
{
    that.m_handle = 0;
}

Shape& Shape::operator=(Shape && that) {
    this->~Shape();

    m_positions = that.m_positions;
    m_normals = that.m_normals;
    m_texCoords = that.m_texCoords;
    m_faces = that.m_faces;
    m_vboTriangleLayout = that.m_vboTriangleLayout;
    m_triangleLayout = that.m_triangleLayout;
    m_numVertices = that.m_numVertices;
    m_handle = that.m_handle;
    m_indexed = that.m_indexed;
    m_built = that.m_built;

    that.m_handle = 0;

    return *this;
}

void Shape::addPosition(float x, float y, float z) {
    m_positions.push_back(x);
    m_positions.push_back(y);
    m_positions.push_back(z);
    m_numVertices++;
}

void Shape::addPositions(const std::vector<float> &positions) {
    for(size_t i = 0; i < positions.size(); i += 3) {
        float x = positions[i];
        float y = positions[i+1];
        float z = positions[i+2];

        addPosition(x, y, z);
    }
}

void Shape::addNormal(float x, float y, float z) {
    m_normals.push_back(x);
    m_normals.push_back(y);
    m_normals.push_back(z);
}

void Shape::addNormals(const std::vector<float> &normals) {
    for(size_t i = 0; i < normals.size(); i += 3) {
        float x = normals[i];
        float y = normals[i+1];
        float z = normals[i+2];

        addNormal(x, y, z);
    }
}

void Shape::addTextureCoordinate(float s, float t) {
    m_texCoords.push_back(s);
    m_texCoords.push_back(t);
}

void Shape::addTextureCoordinates(const std::vector<float> &texCoords) {
    for(size_t i = 0; i < texCoords.size(); i += 2) {
        float s = texCoords[i];
        float t = texCoords[i+1];

        addTextureCoordinate(s, t);
    }
}

void Shape::addVertex(float x, float y, float z, float n1, float n2, float n3, float s, float t) {
    addPosition(x, y, z);
    addNormal(n1, n2, n3);
    addTextureCoordinate(s, t);
}

void Shape::addVertices(const std::vector<float> &vertices) {
    for(size_t i = 0; i < vertices.size(); i += 8) {
        float x = vertices[i];
        float y = vertices[i+1];
        float z = vertices[i+2];
        float n1 = vertices[i+3];
        float n2 = vertices[i+4];
        float n3 = vertices[i+5];
        float s = vertices[i+6];
        float t = vertices[i+7];

        addPosition(x, y, z);
        addNormal(n1, n2, n3);
        addTextureCoordinate(s, t);
    }
}

void Shape::addFace(int v1, int v2, int v3) {
    m_faces.push_back(v1);
    m_faces.push_back(v2);
    m_faces.push_back(v3);

    m_indexed = true;
}

void Shape::addFaces(const std::vector<int> &faces) {
    for(size_t i = 0; i < faces.size(); i += 3) {
        int v1 = faces[i];
        int v2 = faces[i+1];
        int v3 = faces[i+2];

        addFace(v1, v2, v3);
    }
}

void Shape::build() {
    // VBOs
    std::vector<VBOAttribMarker> posMarkers;
    posMarkers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    VBO posVBO(m_positions.data(), m_positions.size(), posMarkers, m_vboTriangleLayout);

    std::vector<VBOAttribMarker> normalMarkers;
    normalMarkers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 0));
    VBO normalVBO(m_normals.data(), m_normals.size(), normalMarkers, m_vboTriangleLayout);

    std::vector<VBOAttribMarker> texMarkers;
    texMarkers.push_back(VBOAttribMarker(ShaderAttrib::TANGENT, 2, 0));
    VBO texVBO(m_texCoords.data(), m_texCoords.size(), texMarkers, m_vboTriangleLayout);

    // IBO
    IBO ibo(m_faces.data(), m_faces.size());

    // VAO
    glGenVertexArrays(1, &m_handle);
    glBindVertexArray(m_handle);

    posVBO.bindAndEnable();
    normalVBO.bindAndEnable();
    texVBO.bindAndEnable();

    if(m_indexed) {
        ibo.bind();
    }

    glBindVertexArray(0);

    posVBO.unbind();
    normalVBO.unbind();
    texVBO.unbind();
    ibo.unbind();

    // Triangle layout
    m_triangleLayout = posVBO.triangleLayout();

    // Number of elements
    if(m_indexed) {
        m_numVertices = m_faces.size();
    }

    // Has been built
    m_built = true;

    // Cleanup
    m_positions.clear();
    m_normals.clear();
    m_texCoords.clear();
    m_faces.clear();
}

void Shape::setShapeName(std::string shape) {
    m_shapeName = shape;

    m_built = (shape != "");
}

std::string Shape::getShapeName() {
    return m_shapeName;
}

void Shape::setPos(glm::vec3 pos) {
    m_pos = pos;
}

void Shape::translate(glm::vec3 delta) {
    m_pos += delta;
}

glm::vec3 Shape::getPos() {
    return m_pos;
}

void Shape::setScale(glm::vec3 scale) {
    m_scale = scale;
}

void Shape::setScale(float scale) {
    m_scale = glm::vec3(scale, scale, scale);
}

void Shape::scale(glm::vec3 scale) {
    m_scale *= scale;
}

void Shape::scale(float scale) {
    m_scale *= scale;
}

glm::vec3 Shape::getScale() {
    return m_scale;
}

void Shape::setRotation(float angle, glm::vec3 axis) {
    m_rotation = glm::angleAxis(glm::degrees(angle), axis);
}

void Shape::setRotation(glm::quat r) {
    m_rotation = r;
}

void Shape::setAngle(float angle) {
    m_rotation = glm::angleAxis(glm::degrees(angle), getAxis());
}

void Shape::setAxis(glm::vec3 axis) {
    m_rotation = glm::angleAxis(getAngle(), axis);
}

void Shape::rotate(float angle, glm::vec3 axis) {
    m_rotation = glm::angleAxis(angle, axis) * m_rotation;
}

void Shape::rotate(glm::quat r) {
    m_rotation = r * m_rotation;
}

void Shape::rotate(float angle) {
    m_rotation = glm::angleAxis(angle + getAngle(), getAxis());
}

float Shape::getAngle() {
    return glm::angle(m_rotation);
}

glm::vec3 Shape::getAxis() {
    return glm::axis(m_rotation);
}

glm::quat Shape::getRotation() {
    return m_rotation;
}

glm::mat4 Shape::getModelMatrix() {
    glm::mat4 model;
    model = glm::translate(model, m_pos);
    model = model * glm::toMat4(m_rotation);
    model = glm::scale(model, m_scale);
    return model;
}
void Shape::draw(const std::shared_ptr<Graphics> &g) {
    draw(g.get());
}

void Shape::draw(Graphics *g) {
    // Build VAO if not buildt already
    if(!m_built) {
        build();
    }

    // Set transform
    glm::mat4 lastTransform = g->getTransform();
    g->pushTransform(getModelMatrix());

    // Draw
    if(m_shapeName == "") {
        glBindVertexArray(m_handle);

        if(!m_indexed) {
            glDrawArrays(m_triangleLayout, 0, m_numVertices);
        }
        else {
            glDrawElements(m_triangleLayout, m_numVertices, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
    }
    else {
        g->drawShape(m_shapeName);
    }

    // Reset transform
    g->setTransform(lastTransform);
}

bool Shape::printDebug() {
    bool noErrors = true;

    if(m_positions.size() != m_normals.size()) {
        std::cerr << "Positions vector has size " << m_positions.size() << " while normals vector has size " << m_normals.size() << std::endl;
        noErrors = false;
    }

    if(m_positions.size() / 3 != m_texCoords.size() / 2) {
        std::cerr << "Positions vector has size " << m_positions.size() << " while texture coordinates vector has size " << m_texCoords.size() << std::endl;
        noErrors = false;
    }

    if(m_triangleLayout == GL_TRIANGLES) {
        if((m_positions.size() % 6) != 0 && (m_faces.size() % 6) != 0) {
            std::cerr << "Using LAYOUT_TRIANGLES, number of vertices should be divisible by 6" << std::endl;
            noErrors = false;
        }
    }

    if(m_triangleLayout == GL_TRIANGLE_STRIP) {
        if((m_positions.size() % 2) != 0 && (m_faces.size() % 2) != 0) {
            std::cerr << "Using LAYOUT_TRIANGLE_STRIP, number of vertices should be divisible by 2" << std::endl;
            noErrors = false;
        }
    }

    return noErrors;
}
