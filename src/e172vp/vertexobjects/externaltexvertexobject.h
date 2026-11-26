// Copyright 2023 Borys Boiko

#pragma once

#include "../descriptorsetlayout.h"
#include "../vertex.h"
#include "abstractvertexobject.h"
#include <vector>

namespace e172vp {

class ExternalTexVertexObject : public AbstractVertexObject {
    struct ubo {
        glm::mat4 model;
    };

    glm::mat4 m_rotation = glm::mat4(1);
    glm::mat4 m_translation = glm::mat4(1);
    glm::mat4 m_scale = glm::mat4(1);

    vk::Buffer m_vertexBuffer;
    vk::DeviceMemory m_vertexBufferMemory;
    vk::Buffer m_indexBuffer;
    vk::DeviceMemory m_indexBufferMemory;
    vk::Buffer m_uniformBuffer;
    vk::DeviceMemory m_uniformBufferMemory;

    std::vector<vk::DescriptorSet> m_descriptorSets;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    vk::ImageView m_imageView;

public:
    ExternalTexVertexObject(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        const vk::ImageView& imageView);
    ~ExternalTexVertexObject();

    void setVertices(const std::vector<Vertex>& vertices);
    glm::mat4 rotation() const;
    void setRotation(const glm::mat4& rotation);
    glm::mat4 translation() const;
    void setTranslation(const glm::mat4& translation);
    glm::mat4 scale() const;
    void setScale(const glm::mat4& scale);

    // AbstractVertexObject interface
public:
    vk::Buffer vertexBuffer() const override;
    vk::Buffer indexBuffer() const override;
    uint32_t indexCount() const override;
    void update(int) override;
    std::vector<vk::DescriptorSet> descriptorSets() override;
    void initialize(
        const GraphicsObject* graphicsObject,
        int,
        const DescriptorSetLayout* objectDSL,
        const DescriptorSetLayout* samplerDSL) override;
};

} // namespace e172vp
