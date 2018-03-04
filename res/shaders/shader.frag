#version 330 core
out vec4 fragColor;

// Additional information for lighting
in vec4 normal_worldSpace;
in vec4 position_worldSpace;

void main() {
    vec4 lightDir = normalize(vec4(-1, -1, -1, 0));
    float l = dot(-normal_worldSpace, lightDir);

    fragColor = vec4(l + 0.1, l + 0.1, l + 0.1, 0);
}
