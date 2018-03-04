#ifndef CUBEDATA_H
#define CUBEDATA_H

#include "GL/glew.h"

// YOU DO NOT NEED TO CHANGE ANYTHING IN HERE

// The amount of distinct vertices in the buffer
int cubeVertexCount = 36;

// The actual size of the array, based on the size of GLfloat and the amount of values
int cubeDataSize = sizeof(GLfloat)*288;

// This is an array containing the Vertex, Normal, and Texture information required to draw a cylinder.
GLfloat cubeVertexBufferData[] = {
    // front
    -0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0,
     0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 0,
     0.5f,  0.5f, 0.5f, 0, 0, 1, 1, 1,
    -0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0,
     0.5f,  0.5f, 0.5f, 0, 0, 1, 1, 1,
    -0.5f,  0.5f, 0.5f, 0, 0, 1, 0, 1,
    // top
    -0.5f, 0.5f,  0.5f, 0, 1, 0, 0, 0,
     0.5f, 0.5f,  0.5f, 0, 1, 0, 1, 0,
     0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1,
    -0.5f, 0.5f,  0.5f, 0, 1, 0, 0, 0,
     0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1,
    -0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1,
    // back
     0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0,
    -0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 0,
    -0.5f,  0.5f, -0.5f, 0, 0, -1, 1, 1,
     0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0,
    -0.5f,  0.5f, -0.5f, 0, 0, -1, 1, 1,
     0.5f,  0.5f, -0.5f, 0, 0, -1, 0, 1,
    // bottom
    -0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0,
     0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 0,
     0.5f, -0.5f,  0.5f, 0, -1, 0, 1, 1,
    -0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0,
     0.5f, -0.5f,  0.5f, 0, -1, 0, 1, 1,
    -0.5f, -0.5f,  0.5f, 0, -1, 0, 0, 1,
    // left
    -0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 0,
    -0.5f, -0.5f,  0.5f, -1, 0, 0, 1, 0,
    -0.5f,  0.5f,  0.5f, -1, 0, 0, 1, 1,
    -0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 0,
    -0.5f,  0.5f,  0.5f, -1, 0, 0, 1, 1,
    -0.5f,  0.5f, -0.5f, -1, 0, 0, 0, 1,
    // right
     0.5f, -0.5f,  0.5f, 1, 0, 0, 0, 0,
     0.5f, -0.5f, -0.5f, 1, 0, 0, 1, 0,
     0.5f,  0.5f, -0.5f, 1, 0, 0, 1, 1,
     0.5f, -0.5f,  0.5f, 1, 0, 0, 0, 0,
     0.5f,  0.5f, -0.5f, 1, 0, 0, 1, 1,
     0.5f,  0.5f,  0.5f, 1, 0, 0, 0, 1,
};

#endif // CUBEDATA_H
