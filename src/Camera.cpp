#include "Camera.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/quaternion.hpp"

Camera::Camera(float fov, float aspect_ratio, float, float)
{
    m_projection = glm::perspective(fov, aspect_ratio, 0.1f, 100.0f);
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Camera::get_view_mat()
{    
    glm::vec3 direction = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f); // Up direction
    glm::mat4 view_mat = glm::lookAt(m_position, m_position + direction, up);
    return view_mat;
}

Camera::~Camera()
{
}
