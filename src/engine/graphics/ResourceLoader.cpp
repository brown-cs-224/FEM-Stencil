#include "ResourceLoader.h"
#include <QFile>
#include <QTextStream>

#include "engine/util/CommonIncludes.h"
#include "engine/graphics/Shape.h"

ResourceLoader::ResourceLoader(){}

GLuint ResourceLoader::loadShaders(const char * vertex_file_path, const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    QString vertFilePath = QString(vertex_file_path);
    QFile vertFile(vertFilePath);
    if (vertFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream vertStream(&vertFile);
        VertexShaderCode = vertStream.readAll().toStdString();
        vertFile.close();
    }

    // Read fragment shader code from file
    std::string FragmentShaderCode;
    QString fragFilePath = QString(fragment_file_path);
    QFile fragFile(fragFilePath);
    if (fragFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream fragStream(&fragFile);
        FragmentShaderCode = fragStream.readAll().toStdString();
        fragFile.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!Result && InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stderr, "Error compiling shader: %s\n%s\n",
                vertex_file_path, &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!Result && InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stderr, "Error compiling shader: %s\n%s\n",
                fragment_file_path, &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, VertexShaderID);
    glAttachShader(programId, FragmentShaderID);
    glLinkProgram(programId);

    // Check the program
    glGetProgramiv(programId, GL_LINK_STATUS, &Result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (!Result && InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength);
        glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stderr, "Error linking shader: %s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return programId;
}

std::string ResourceLoader::loadResourceFileToString(const std::string &resourcePath)
{
    QString vertFilePath = QString::fromStdString(resourcePath);
    QFile vertFile(vertFilePath);

    if (vertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream vertStream(&vertFile);
        QString contents = vertStream.readAll();
        return contents.toStdString();
    }

    throw Resource::IOException("Could not open file: " + resourcePath);
}

bool ResourceLoader::parseFaceVertex(const QString &vertString, IndexTuple &vertex) {
    bool success = true;

    // Position and normal indices without texture coordinate indices
    if(vertString.contains("//"))
    {
        QStringList parts = vertString.split("//");
        assert(parts.size() == 2);
        vertex[0] = QString(parts[0]).toUInt(&success);
        vertex[2] = QString(parts[1]).toUInt(&success);
    }
    else if(vertString.contains("/"))
    {
        QStringList parts = vertString.split("/");

        // Position and texcoord mixed
        if(parts.size() == 2)
        {
            vertex[0] = QString(parts[0]).toUInt(&success);
            vertex[1] = QString(parts[1]).toUInt(&success);
        }
        // Position, texcoord, and normal mixed
        else if(parts.size() == 3)
        {
            vertex[0] = QString(parts[0]).toUInt(&success);
            vertex[1] = QString(parts[1]).toUInt(&success);
            vertex[2] = QString(parts[1]).toUInt(&success);
        }
        else
        {
            success = false;
        }
    }
    // Nothing mixed
    else
    {
        vertex[0] = vertString.toUInt(&success);
    }

    return success;
}

bool ResourceLoader::readObj(const QString &path, std::shared_ptr<Shape> &shape) {
    // Open the file
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "ERROR: Could not read OBJ file." << std::endl;
        return false;
    }

    // Vertices normals and texture coordinates
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<IndexTuple> faces;

    // Used if positions, normals, and texture coordinates with different indices are mixed
    IndexTuple defaultTuple;
    defaultTuple.resize(3,0);

    // Read file
    QTextStream f(&file);
    QString line;
    QRegExp spaces("\\s+");

    while(!line.isNull())
    {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;

        // vertex position
        if (parts[0] == "v" && parts.count() >= 4)
        {
            const glm::vec3 pos = glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
            positions.push_back(pos);
        }
        // vertex texture coordinate
        else if (parts[0] == "vt")
        {
            const glm::vec2 coord = glm::vec2(parts[1].toFloat(), parts[2].toFloat());
            texCoords.push_back(coord);
        }
        // vertex normal
        else if(parts[0] == "vn")
        {
            const glm::vec3 normal = glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
            normals.push_back(normal);
        }
        // triangle face
        else if(parts[0] == "f")
        {
            int numFaces = faces.size();
            faces.resize(numFaces + 3, defaultTuple);

            bool success = true;
            success &= parseFaceVertex(parts[1], faces[numFaces]);
            success &= parseFaceVertex(parts[2], faces[numFaces + 1]);
            success &= parseFaceVertex(parts[3], faces[numFaces + 2]);
            assert(success);

            if(!success) {
                return false;
            }
        }
    }

    // Accumulate vertex data from arrays
    std::vector<float> vertices = std::vector<float>(faces.size() * 8);
    auto verticesIt = vertices.begin();

    for(auto facesIt = faces.begin(); facesIt != faces.end(); ++facesIt)
    {
        // Get data corresponding to current face
        const IndexTuple i = *facesIt;
        glm::vec3 pos;
        glm::vec2 texc;
        glm::vec3 norm;

        // pos
        pos = positions.at(i[0]-1);

        // texcoord
        if(!texCoords.empty())
        {
            texc = texCoords.at(i[1]-1);
        }

        // normal
        if(!normals.empty())
        {
            norm = normals.at(i[2]-1);
        }

        float vert[8] =
        {
            pos.x, pos.y, pos.z,
            norm.x, norm.y, norm.z,
            texc.x, texc.y
        };

        std::copy(std::begin(vert), std::end(vert), verticesIt);
        std::advance(verticesIt, 8);
    }

    // Return shape
    shape = std::make_shared<Shape>(vertices);
    return true;
}
