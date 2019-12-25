#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
};

in vec3 FragPos;
in vec3 Normal;

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 color;



void main()
{
    // AMBIENT
    vec3 ambient =  light.ambient * material.ambient;

    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 objToLightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, objToLightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightToObjDir = -objToLightDir;
    vec3 reflectDir = reflect(lightToObjDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec;

    vec3 result = min(vec3(1.0), ambient + diffuse + specular);
    color = vec4(result, 1.0);
//    color = vec4(lightColor * objectColor, 1.0f);
}