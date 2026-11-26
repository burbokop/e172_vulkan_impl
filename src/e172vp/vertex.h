// Copyright 2023 Borys Boiko

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace e172vp {

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;

    static std::vector<Vertex> fromGlm(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::vec2>& texCoords = std::vector<glm::vec2>(),
        glm::vec3 color = glm::vec3(1.));
    static vk::VertexInputBindingDescription bindingDescription();
    static std::array<
        vk::VertexInputAttributeDescription,
        3>
    attributeDescriptions();
};

bool operator==(
    const Vertex& v0,
    const Vertex& v1);

} // namespace e172vp
