#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <vector>
#include <string>
#include "glm.hpp"

class MeshLoader
{
public:
    static bool loadObj(const std::string &filepath, std::vector<glm::vec3> &vertices, std::vector<glm::ivec3> &faces);
    static bool loadTetMesh(const std::string &filepath, std::vector<glm::vec3> &vertices, std::vector<glm::ivec4> &tets);
private:
    MeshLoader();
};

#endif // MESHLOADER_H
