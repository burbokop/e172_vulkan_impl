// Copyright 2023 Borys Boiko

#pragma once

#include "../vertex.h"
#include "abstractvertexobject.h"
#include <glm/glm.hpp>
#include <vector>

namespace e172vp {

class TexturedVertexObject : public AbstractVertexObject {
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

    vk::Image m_image;
    vk::DeviceMemory m_imageMemory;
    vk::ImageView m_imageView;

    int m_textureWidth;
    int m_textureHeight;

public:
    TexturedVertexObject(
        int textureWidth,
        int textureHeight);

    ~TexturedVertexObject();

    // AbstractVertexObject interface
public:
    void initialize(
        const GraphicsObject* graphicsObject,
        int frameCount,
        const DescriptorSetLayout* objectDSL,
        const DescriptorSetLayout* samplerDSL) override;
    vk::Buffer vertexBuffer() const override;
    vk::Buffer indexBuffer() const override;
    uint32_t indexCount() const override;
    void update(int) override;
    std::vector<vk::DescriptorSet> descriptorSets() override;
    int textureWidth() const;
    int textureHeight() const;
};

} // namespace e172vp
