#pragma once

#include <e172/utility/buffer.h>
#include <e172/utility/either.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace e172::impl::vulkan {
class Renderer;
}

namespace e172vp {

class Mesh
{
    friend e172::impl::vulkan::Renderer; // TODO: remove it

public:
    enum Error { FileNotFound };

    /**
     * @brief parseObj - parse string containing .obj format data
     */
    static e172::Either<Error, Mesh> parseObj(const std::string &data);

    /**
     * @brief parseObj - parse string containing .obj format data
     */
    static e172::Either<Error, Mesh> parseObj(const e172::ByteRange &data);

    /**
     * @brief loadObj - load .obj file
     */
    static e172::Either<Error, Mesh> loadObj(const std::filesystem::path &path);

    /**
     * @brief plate - create plate mesh
     */
    static Mesh plate(float radius);

    /**
     * @brief plate - create plate mesh
     */
    static Mesh plate(const glm::vec2 point0, const glm::vec2 point1);

    const std::vector<glm::vec3> &vertices() const { return m_vertices; };
    const std::vector<glm::vec2> &uvMap() const { return m_uvMap; };
    const std::vector<glm::vec3> &normals() const { return m_normals; };

    const std::vector<uint32_t> &vertexIndices() const { return m_vertexIndices; };
    const std::vector<uint32_t> &uvIndices() const { return m_uvIndices; };
    const std::vector<uint32_t> &normalIndices() const { return m_normalIndices; };

    bool hasTexture() const { return m_hasTexture; }

    Mesh(Mesh &&) = default;
    Mesh(const Mesh &) = default;
    Mesh &operator=(const Mesh &) = default;

private:
    static e172::Either<Error, Mesh> parseObjFromCStream(FILE *stream);

    Mesh() = default;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_uvMap;
    std::vector<glm::vec3> m_normals;

    std::vector<uint32_t> m_vertexIndices;
    std::vector<uint32_t> m_uvIndices;
    std::vector<uint32_t> m_normalIndices;

    bool m_hasTexture = false;
};

} // namespace e172vp
