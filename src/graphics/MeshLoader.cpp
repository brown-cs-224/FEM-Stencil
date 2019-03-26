#include "MeshLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj/tiny_obj_loader.h>

#include <iostream>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>


bool MeshLoader::loadObj(const std::string &filepath, std::vector<glm::vec3> &vertices, std::vector<glm::ivec3> &faces)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                                filepath.c_str(), nullptr, true);
    if(!ret) {
        std::cout << err << std::endl;
        return false;
    }
    for(size_t s = 0; s < shapes.size(); ++s) {
        size_t indexOffset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            glm::ivec3 face;
            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                face[v] = idx.vertex_index;
            }
            faces.push_back(face);

            indexOffset += fv;
        }
    }
    for(size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    return true;
}

bool MeshLoader::loadTetMesh(const std::string &filepath, std::vector<glm::vec3> &vertices, std::vector<glm::ivec4> &tets)
{
    QString qpath = QString::fromStdString(filepath);
    QFile file(qpath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "Error opening file: " << filepath << std::endl;
        return false;
    }
    QTextStream in(&file);

    QRegularExpression vrxp("v (-?\\d*\\.?\\d+) +(-?\\d*\\.?\\d+) +(-?\\d*\\.?\\d+)");
    QRegularExpression trxp("t (\\d+) +(\\d+) +(\\d+) +(\\d+)");

    while(!in.atEnd()) {
        QString line = in.readLine();
        auto match = vrxp.match(line);
        if(match.hasMatch()) {
            vertices.emplace_back(match.captured(1).toFloat(),
                                  match.captured(2).toFloat(),
                                  match.captured(3).toFloat());
            continue;
        }
        match = trxp.match(line);
        if(match.hasMatch()) {
            tets.emplace_back(match.captured(1).toInt(),
                              match.captured(2).toInt(),
                              match.captured(3).toInt(),
                              match.captured(4).toInt());
        }
    }
    file.close();
    return true;
}

MeshLoader::MeshLoader()
{

}
