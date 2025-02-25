#pragma once
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/quaternion.hpp"
#include <vector>

template<typename VertexType>
class Object
{
public:
    Object();
    ~Object();
private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    std::vector<VertexType> m_vertices;
    static size_t m_vertex_type_size = sizeof(VertexType);
};