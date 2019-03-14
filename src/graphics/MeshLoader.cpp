#include "MeshLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj/tiny_obj_loader.h>

#include <iostream>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

using namespace Eigen;

bool MeshLoader::loadTetMesh(const std::string &filepath, std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector4i> &tets)
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
