#version 330 core
out vec4 fragColor;

// Additional information for lighting
in vec4 normal_worldSpace;
in vec4 position_worldSpace;

uniform int wire = 0;

void main() {
    if(wire == 1) {
        fragColor = vec4(1, 1, 1, 1);
        return;
    }
    vec4 lightDir = normalize(vec4(-1, -1, 1, 0));
    float l = clamp(dot(-normal_worldSpace, lightDir), 0, 1);

    float c = min(l + 0.2, 1);
    fragColor = vec4(c, c, c, 1);
}
