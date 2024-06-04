#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos;

void main() {
    vec3 lightColor = vec3(1, 1, 1);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 rgba = texture(ourTexture, TexCoord);
    vec3 color = vec3(rgba.x, rgba.y, rgba.z);

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1.0);
}