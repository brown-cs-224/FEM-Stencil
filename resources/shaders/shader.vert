#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex

uniform mat3 invTransposeM;
uniform mat4 mvp;

out vec4 normal_worldSpace;
out vec4 position_worldSpace;

void main() {
    normal_worldSpace   = vec4(normalize(invTransposeM * normal), 0);
    position_worldSpace = vec4(position, 1.0);

    gl_Position = mvp * vec4(position, 1.0);
}
