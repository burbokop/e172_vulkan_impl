// Copyright 2023 Borys Boiko

#pragma once

#include "../vertex.h"
#include "abstractvertexobject.h"
#include <vector>

namespace e172vp {

class WireVertexObject : public AbstractVertexObject {
    vk::Buffer m_vertexBuffer;
    vk::DeviceMemory m_vertexBufferMemory;
    vk::Buffer m_indexBuffer;
    vk::DeviceMemory m_indexBufferMemory;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

public:
    WireVertexObject(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices);
    ~WireVertexObject();

    void setVertices(const std::vector<e172vp::Vertex>& vertices);

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
        const DescriptorSetLayout*,
        const DescriptorSetLayout*) override;
};

} // namespace e172vp
