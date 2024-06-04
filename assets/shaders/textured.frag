#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 viewPos;

void main() {
    vec3 lightColor = vec3(1, 1, 1);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    vec4 rgba = texture(ourTexture, TexCoord);
    vec3 color = vec3(rgba.x, rgba.y, rgba.z);

    vec3 ambient = material.ambient * light.ambient;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);

    // NOTE: to debug normals: FragColor = vec4(norm, 1.0);
}