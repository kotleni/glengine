#version 330 core

out vec3 color;
in vec3 fragPos;

void main(){
    float mixValue = distance(fragPos, vec3(0, 1, 0));

    vec3 rgb1 = vec3(0, 0.4, 0.6);
    vec3 rgb2 = vec3(0.3, 0.4, 0.2);

    vec3 mixed = mix(rgb1, rgb2, mixValue);

    color = mixed;
}