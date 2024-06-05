#version 330 core

struct Features {
    bool is_use_light;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform Features features;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 viewPos;

vec3 applyLight(vec3 color, vec3 norm) {
    vec3 lightColor = vec3(1, 1, 1);

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    vec3 ambient = material.ambient * light.ambient;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    return (ambient + diffuse + specular) * color;
}

void main() {
    vec3 norm = normalize(Normal);

    vec4 rgba = texture(ourTexture, TexCoord);
    vec3 color = vec3(rgba.x, rgba.y, rgba.z);
    
    if(features.is_use_light)
        FragColor = vec4(applyLight(color, norm), 1.0);
    else
        FragColor = vec4(color, 1.0);
    
    // NOTE: to debug normals: FragColor = vec4(norm, 1.0);
}