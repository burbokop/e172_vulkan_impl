#include "mesh.h"

#include <string.h>

e172vp::Mesh e172vp::Mesh::plate(float radius)
{
    Mesh result;
    result.m_hasTexture = true;
    result.m_vertices = {{-radius, -radius, 0},
                         {radius, -radius, 0},
                         {radius, radius, 0},
                         {-radius, radius, 0}};
    result.m_uvMap = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    result.m_vertexIndices = {0, 1, 2, 2, 3, 0};
    return result;
}

e172vp::Mesh e172vp::Mesh::plate(const glm::vec2 point0, const glm::vec2 point1) {
    Mesh result;
    result.m_hasTexture = true;
    result.m_vertices = {{point0.x, point0.y, 0},
                         {point1.x, point0.y, 0},
                         {point1.x, point1.y, 0},
                         {point0.x, point1.y, 0}};
    result.m_uvMap = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    result.m_vertexIndices = {0, 1, 2, 2, 3, 0};
    return result;
}

e172::Either<e172vp::Mesh::Error, e172vp::Mesh> e172vp::Mesh::parseObjFromCStream(FILE *stream)
{
    assert(stream);
    e172vp::Mesh result;
    result.m_hasTexture = true;

    while (true) {
        std::string line;
        if (fscanf(stream, "%s", line.data()) == EOF)
            break;
        else if (strcmp(line.c_str(), "v") == 0) {
            float x = 0;
            float y = 0;
            float z = 0;
            fscanf(stream, "%f %f %f\n", &x, &y, &z);
            result.m_vertices.push_back({x, y, z});
        } else if (strcmp(line.c_str(), "vt") == 0) {
            float x = 0;
            float y = 0;
            fscanf(stream, "%f %f\n", &x, &y);
            result.m_uvMap.push_back({x, y});
        }

        else if (strcmp(line.c_str(), "vn") == 0) {
            float x = 0;
            float y = 0;
            float z = 0;
            fscanf(stream, "%f %f %f\n", &x, &y, &z);
            result.m_normals.push_back({x, y, z});
        } else if (strcmp(line.c_str(), "f") == 0) {
            uint32_t iv1x = 0;
            uint32_t iv1y = 0;
            uint32_t iv1z = 0;
            uint32_t iv2x = 0;
            uint32_t iv2y = 0;
            uint32_t iv2z = 0;
            uint32_t iv3x = 0;
            uint32_t iv3y = 0;
            uint32_t iv3z = 0;
            fscanf(stream,
                   "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                   &iv1x,
                   &iv1y,
                   &iv1z,
                   &iv2x,
                   &iv2y,
                   &iv2z,
                   &iv3x,
                   &iv3y,
                   &iv3z);

            result.m_vertexIndices.push_back(iv1x - 1);
            result.m_vertexIndices.push_back(iv2x - 1);
            result.m_vertexIndices.push_back(iv3x - 1);

            result.m_uvIndices.push_back(iv1y - 1);
            result.m_uvIndices.push_back(iv2y - 1);
            result.m_uvIndices.push_back(iv3y - 1);

            result.m_normalIndices.push_back(iv1z - 1);
            result.m_normalIndices.push_back(iv2z - 1);
            result.m_normalIndices.push_back(iv3z - 1);
        }
    }
    return e172::Right(result);
}

e172::Either<e172vp::Mesh::Error, e172vp::Mesh> e172vp::Mesh::parseObj(const std::string &data)
{
    auto stream = ::fmemopen(const_cast<char *>(data.data()), data.size(), "r");
    const auto result = parseObjFromCStream(stream);
    ::fclose(stream);
    return result;
}

e172::Either<e172vp::Mesh::Error, e172vp::Mesh> e172vp::Mesh::parseObj(const e172::ByteRange &data)
{
    return parseObj(std::string(data.begin(), data.end()));
}

e172::Either<e172vp::Mesh::Error, e172vp::Mesh> e172vp::Mesh::loadObj(
    const std::filesystem::path &path)
{
    auto stream = ::fopen(path.c_str(), "r");
    if (!stream) {
        return e172::Left(FileNotFound);
    }
    const auto result = parseObjFromCStream(stream);
    ::fclose(stream);
    return result;
}
