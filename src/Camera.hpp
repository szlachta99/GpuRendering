#pragma once

#include <SDL3/SDL.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/quaternion.hpp"

class Camera
{
public:
    Camera() : m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f, 0.0f, 0.0f, 1.0f) {}
    Camera(float fov, float aspect_ratio, float near, float far);
    void set_position(glm::vec3 position) { m_position = position; };
    void set_rotation(glm::quat rotation) { m_rotation = rotation; };
    glm::mat4 get_view_mat(); 
    ~Camera();
private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::mat4 m_projection;
};