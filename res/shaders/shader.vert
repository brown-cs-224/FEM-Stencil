#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex

uniform mat4 m;
uniform mat4 vp;

out vec4 normal_worldSpace;
out vec4 position_worldSpace;

void main(){
    normal_worldSpace = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    position_worldSpace = vec4(position, 1.0);
    gl_Position = vp * m * vec4(position, 1.0);
}
