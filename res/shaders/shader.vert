#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 2) in vec2 texCoord; // UV texture coordinates

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec4 normal_worldSpace;
out vec4 position_worldSpace;
out vec4 eye_worldSpace;
out vec2 texc;

void main(){
    texc = texCoord;
    position_worldSpace = m * vec4(position, 1.0);
    normal_worldSpace = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    eye_worldSpace = inverse(v)*vec4(0,0,0,1);
    gl_Position = p * v * m * vec4(position, 1.0);
}
