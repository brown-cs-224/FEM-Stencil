#version 330 core
out vec4 fragColor;

struct Material
{
    int useLighting;
    vec3 color;
    float alpha;
    vec3 specularColor;
    float shininess;

    int useTexture;
    vec2 texRepeat;
    vec2 texStart;
    vec2 texEnd;
};

struct FontMaterial
{
    int isFont;
    vec2 fontTexStart;
    vec2 fontTexEnd;
};

struct Light
{
    int type;
    vec3 color;
    vec3 dir;
    vec3 pos;
    vec2 att;
    float radius;
};

// Materials
uniform sampler2D tex;
uniform sampler2D fontTex;
uniform Material material = Material(1, vec3(1), 1, vec3(0), 1, 1, vec2(1), vec2(0), vec2(1));
uniform FontMaterial font = FontMaterial(0, vec2(0), vec2(1));

// Lighting
const int MAX_NUM_LIGHTS = 1;
uniform int numLights = 0;
uniform Light lights[MAX_NUM_LIGHTS];

// Additional information for lighting
in vec4 normal_worldSpace;
in vec4 position_worldSpace;
in vec4 eye_worldSpace;
in vec2 texc;

vec3 get_contribution_ambient(Light light, vec3 material_color) {
    return light.color * material_color;
}

vec3 get_contribution(vec3 light_dir, vec3 light_color, vec3 material_color) {
    vec3 result = vec3(0);

    // Diffuse lighting
    vec4 vertexToLight = vec4(-light_dir, 0); // Direction from world space location of current pixel to the light source
    float diffuseIntensity = max(0, dot(vertexToLight, normal_worldSpace)); // Attenuate light intensity according to cosine of angle between surface and light
    vec3 diffuseColor = light_color * material_color * diffuseIntensity;
    result += diffuseColor;

    // Specular lighting
    vec4 lightReflection = normalize(-reflect(vertexToLight, normal_worldSpace)); // Light direction reflected about vertex normal
    vec4 eyeDirection = normalize(eye_worldSpace - position_worldSpace); // Vertex to eye location
    float specularIntensity = pow(max(0, dot(eyeDirection, lightReflection)), 150 * material.shininess); // Increase light intensity if reflected light aligns with direction from vertex to eye
    vec3 specularColor =  light_color * material.specularColor * specularIntensity;
    result += specularColor;

    return result;
}

vec3 get_contribution_directional(Light light, vec3 material_color) {
    return get_contribution(normalize(light.dir), light.color, material_color);
}

vec3 get_contribution_point(Light light, vec3 material_color) {
    vec3 res = get_contribution(normalize(position_worldSpace.xyz - light.pos), light.color, material_color);

    float d = distance(light.pos, position_worldSpace.xyz);
    float att = 1.0 / (1.0 + light.att.x * d + light.att.y * d * d);
    return att * res;
}

vec3 get_contribution(Light light, vec3 material_color) {
    if(light.type == 0) {
        return get_contribution_ambient(light, material_color);
    }
    else if(light.type == 1) {
        return get_contribution_directional(light, material_color);
    }
    else {
        return get_contribution_point(light, material_color);
    }
}

void main(){
    fragColor = vec4(0); // Color that will be displayed
    vec3 material_color = vec3(0); // Material color
    float a = 1; // Material alpha value (transparency)

    // Get material color from texture
    if(material.useTexture > 0) {
        vec2 tc = fract(texc * material.texRepeat);
        tc = material.texStart + tc * (material.texEnd - material.texStart);
        vec4 texColor = clamp(texture(tex, tc), 0, 1); // Get base color from texture

        material_color = texColor.rgb;
        a = material.alpha * texColor.a;
    }
    // Get material color from color uniform
    else {
        material_color = material.color;
        a =  material.alpha;
    }

    // Update alpha if is a font
    if(font.isFont > 0) {
        vec2 ftc = fract(texc);
        ftc = font.fontTexStart + ftc * (font.fontTexEnd - font.fontTexStart);
        vec4 fontTexColor = clamp(texture(fontTex, ftc), 0, 1);

        a = a * fontTexColor.r;
    }

    // Do lighting computations
    if(material.useLighting > 0) {
        if(numLights > 0) {
            int i;

            for(i = 0; i < MAX_NUM_LIGHTS; i++) {
                if(i >= numLights) {
                    break;
                }

                fragColor.rgb += get_contribution(lights[i], material_color);
            }
        }
        else {
            fragColor.rgb += get_contribution(Light(1, vec3(1), vec3(-1), vec3(0), vec2(0), 0), material_color);
            fragColor.rgb += get_contribution(Light(0, vec3(0.3), vec3(-1), vec3(0), vec2(0), 0), material_color);
        }

        fragColor.a = a;
    }
    else {
        fragColor.rgb += material_color;
        fragColor.a = a;
    }
}
