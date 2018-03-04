#ifndef FILELOADER_H
#define FILELOADER_H

#include "CommonIncludes.h"
#include "src/engine/graphics/Shape.h"
#include <string>
#include <tuple>
#include <QFile>
#include <QTextStream>

//represents vertex,texc,normal indices
using IndexTuple = std::vector<size_t>;

class FileLoader
{

public:

    static void parseFaceVertex(const QString &vertString, IndexTuple &vertex, bool *success)
    {
        //Vertex and Vertex Normal Indices Without Texture Coordinate Indices
        if(vertString.contains("//"))
        {
            QStringList parts = vertString.split("//");
            assert(parts.size() == 2);
            vertex[0] = QString(parts[0]).toUInt(success);
            vertex[2] = QString(parts[1]).toUInt(success);
        }
        else if(vertString.contains("/"))
        {
            QStringList parts = vertString.split("/");

            //vertex and texc
            if(parts.size() == 2)
            {
                vertex[0] = QString(parts[0]).toUInt(success);
                vertex[1] = QString(parts[1]).toUInt(success);
            }
            //vertex, texc, and normal
            else if(parts.size() == 3)
            {
                vertex[0] = QString(parts[0]).toUInt(success);
                vertex[1] = QString(parts[1]).toUInt(success);
                vertex[2] = QString(parts[1]).toUInt(success);
            }
            else
            {
                *success = false;
            }
        }
        //just vertex
        else
        {
            vertex[0] = vertString.toUInt(success);
        }
    }

    static void readObj(const QString &path, Shape &loadedShape)
    {
        // Open the file
        QFile file(path);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            std::cout << "ERROR: File could not be read." << std::endl;
            return;
        }

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<IndexTuple> faceVerts;
        size_t numFaceVerts = 0;
        IndexTuple def;
        def.resize(3,0);

        QTextStream f(&file);
        QString line;
        QRegExp spaces("\\s+");
        do
        {
            line = f.readLine().trimmed();
            QStringList parts = line.split(spaces);
            if (parts.isEmpty()) continue;

            //geometry vertex
            if (parts[0] == "v" && parts.count() >= 4)
            {
                const glm::vec3 vert = glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
                vertices.push_back(vert);
            }
            //texture vertex
            else if (parts[0] == "vt")
            {
                const glm::vec2 coord = glm::vec2(parts[1].toFloat(), parts[2].toFloat());
                texCoords.push_back(coord);
            }
            //vertex normal
            else if(parts[0] == "vn")
            {
                const glm::vec3 normal = glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
                normals.push_back(normal);
            }
            //triangle face
            else if(parts[0] == "f")
            {
                bool success;
                faceVerts.resize(numFaceVerts + 3, def);
                parseFaceVertex(parts[1], faceVerts[numFaceVerts]  , &success);
                parseFaceVertex(parts[2], faceVerts[numFaceVerts+1], &success);
                parseFaceVertex(parts[3], faceVerts[numFaceVerts+2], &success);
                assert(success);
                numFaceVerts +=3;
            }
        }
        while (!line.isNull());

//        if(vertices.size() ^ texCoords.size() ^ normals.size())
//        {
//            std::cerr << "Error parsing obj file: File contains a different number of vertices, normals, and texture coordinates. Resulting Shape may be invalid.\n";
//        }

        std::vector<float> vertexData = std::vector<float>(faceVerts.size() * 8);
        auto dataIt = vertexData.begin();
        for(auto faceVertIt = faceVerts.begin(); faceVertIt != faceVerts.end(); ++faceVertIt)
        {
            const IndexTuple i = *faceVertIt;
            const glm::vec3 vert = vertices.at(i[0]-1);
            glm::vec2 texc;
            glm::vec3 norm;
            if(!texCoords.empty())
            {
                texc = texCoords.at(i[1]-1);
            }
            if(!normals.empty())
            {
                norm = normals.at(i[2]-1);
            }

            float point[8] =
            {
                vert.x,vert.y,vert.z,
                norm.x,norm.y,norm.z,
                texc.x,texc.y
            };
            std::copy(std::begin(point), std::end(point), dataIt);
            std::advance(dataIt,8);
        }

        loadedShape = Shape(vertexData);
    }

};

#endif // FILELOADER_H
