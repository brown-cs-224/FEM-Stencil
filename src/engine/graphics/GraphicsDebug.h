#ifndef GRAPHICSDEBUG_H
#define GRAPHICSDEBUG_H

#include "GL/glew.h"
#include <string>

#define GRAPHICS_DEBUG_LEVEL 0

void checkError(std::string prefix = "");
void printGLErrorCodeInEnglish(GLenum err);

void checkFramebufferStatus();
void printFramebufferErrorCodeInEnglish(GLenum err);

void checkShaderCompilationStatus(GLuint shaderID);
void checkShaderLinkStatus(GLuint shaderProgramID);

#endif // GLDEBUG_H
