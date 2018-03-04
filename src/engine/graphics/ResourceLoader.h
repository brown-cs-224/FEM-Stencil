#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "GL/glew.h"

#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <QString>

class Shape;
using IndexTuple = std::vector<size_t>;

namespace Resource {
class IOException : public std::exception {
public:
    IOException(const std::string &what) : message(what) {}
    virtual ~IOException() throw() {}
    virtual const char* what() const throw() override { return message.c_str(); }

private:
    std::string message;
};
}

/**
 * A helper class for loading and compiling shaders.
 *
 * @brief The ResourceLoader class
 */
class ResourceLoader
{
public:
    ResourceLoader();
    static GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
    static std::string loadResourceFileToString(const std::string &resourcePath);

    static bool parseFaceVertex(const QString &vertString, IndexTuple &vertex);
    static bool readObj(const QString &path, std::shared_ptr<Shape> &shape);
};

#endif // RESOURCELOADER_H
