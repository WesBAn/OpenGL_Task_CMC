#version 330 core

const int SUN_LIGHT_MODE = 0;
const int DIRECT_LIGHT_MODE = 1;
const int FULL_MODE = 2;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SunLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirectLight light;
uniform SunLight sun;

uniform int lightMode = FULL_MODE;

out vec4 color;

vec4 sunModeColor()
{
    // AMBIENT
    vec3 ambient = sun.ambient * vec3(texture(material.diffuse, TexCoords));

    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 objToLightDir = normalize(-sun.direction);
    float diff = max(dot(norm, objToLightDir), 0.0);
    vec3 diffuse = sun.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightToObjDir = -objToLightDir;
    vec3 reflectDir = reflect(lightToObjDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = sun.specular * vec3(texture(material.specular, TexCoords)) * spec;

    vec3 result = min(vec3(1.0), ambient + diffuse + specular);
    return vec4(result, 1.0);
}

vec4 directModeColor()
{
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // AMBIENT
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation;

    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 objToLightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, objToLightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightToObjDir = -objToLightDir;
    vec3 reflectDir = reflect(lightToObjDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoords)) * spec * attenuation;

    vec3 result = min(vec3(1.0f), ambient + diffuse + specular);
    return vec4(result, 1.0f);
}



void main()
{
    if (lightMode == FULL_MODE) {
        color = min(vec4(1.0f), sunModeColor() + directModeColor());
    } else if (lightMode == SUN_LIGHT_MODE) {
        color = min(vec4(1.0f), sunModeColor());
    } else if (lightMode == DIRECT_LIGHT_MODE) {
        color = min(vec4(1.0f), directModeColor());
    }
}