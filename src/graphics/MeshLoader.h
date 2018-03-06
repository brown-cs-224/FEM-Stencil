#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <vector>
#include <Eigen/Dense>
#include <Eigen/StdVector>

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Vector4i)

class MeshLoader
{
public:
    static bool loadObj(const std::string &filepath, std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3i> &faces);
    static bool loadTetMesh(const std::string &filepath, std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector4i> &tets);
private:
    MeshLoader();
};

#endif // MESHLOADER_H
