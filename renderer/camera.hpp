#ifndef H_CAMERA
#define H_CAMERA

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"

class Camera {
private:
    float yaw;
    float pitch;

    glm::mat4 proj;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:
    Camera(glm::vec2 render_size);
    void update();
    void applyToShader(Shader* shader);
    
    void look_relative(float xrel, float yrel);
    void move_forward(float step);
    void move_right(float step);

    glm::mat4 get_projection();
    glm::mat4 get_view();
    glm::vec3 get_position();
};

#endif