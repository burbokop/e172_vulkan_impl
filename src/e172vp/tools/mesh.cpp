// Copyright 2023 Borys Boiko

#include "mesh.h"

#include <string.h>
#include <string>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#endif

namespace e172vp {

namespace {

#ifdef _WIN32

FILE* win_fmemopen(
    void* buf,
    size_t size,
    const char* mode)
{
    char temp_path[MAX_PATH];
    char temp_file[MAX_PATH];

    if (!GetTempPathA(MAX_PATH, temp_path) || !GetTempFileNameA(temp_path, "fmem", 0, temp_file)) {
        return NULL;
    }

    // Create a temporary file that is automatically deleted when closed
    HANDLE hFile = CreateFileA(
        temp_file,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    // Write the initial buffer contents into the temporary file
    if (buf != NULL && size > 0 && mode[0] != 'w') {
        DWORD written = 0;
        if (!WriteFile(hFile, buf, (DWORD)size, &written, NULL) || written != size) {
            CloseHandle(hFile);
            return NULL;
        }
        // Reset file pointer to the beginning for reading (unless appending)
        if (mode[0] != 'a') {
            SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        }
    }

    // Convert the Win32 HANDLE to a C runtime file descriptor, then to FILE*
    int fd = _open_osfhandle((intptr_t)hFile, _O_RDWR | _O_BINARY);
    if (fd == -1) {
        CloseHandle(hFile);
        return NULL;
    }

    FILE* fp = _fdopen(fd, mode);
    if (!fp) {
        _close(fd); // Also closes hFile
        return NULL;
    }

    return fp;
}

#endif

}

Mesh Mesh::plate(float radius)
{
    Mesh result;
    result.m_hasTexture = true;
    result.m_vertices = {
        { -radius, -radius, 0 }, { radius, -radius, 0 }, { radius, radius, 0 }, { -radius, radius, 0 }
    };
    result.m_uvMap = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    result.m_vertexIndices = { 0, 1, 2, 2, 3, 0 };
    return result;
}

Mesh Mesh::plate(
    const glm::vec2 point0,
    const glm::vec2 point1)
{
    Mesh result;
    result.m_hasTexture = true;
    result.m_vertices = {
        { point0.x, point0.y, 0 }, { point1.x, point0.y, 0 }, { point1.x, point1.y, 0 }, { point0.x, point1.y, 0 }
    };
    result.m_uvMap = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    result.m_vertexIndices = { 0, 1, 2, 2, 3, 0 };
    return result;
}

e172::Either<
    Mesh::Error,
    Mesh>
Mesh::parseObjFromCStream(FILE* stream)
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
            const auto res = fscanf(stream, "%f %f %f\n", &x, &y, &z);
            if (res == EOF) {
                std::abort();
            }

            result.m_vertices.push_back({ x, y, z });
        } else if (strcmp(line.c_str(), "vt") == 0) {
            float x = 0;
            float y = 0;
            const auto res = fscanf(stream, "%f %f\n", &x, &y);
            if (res == EOF) {
                std::abort();
            }

            result.m_uvMap.push_back({ x, y });
        }

        else if (strcmp(line.c_str(), "vn") == 0) {
            float x = 0;
            float y = 0;
            float z = 0;
            const auto res = fscanf(stream, "%f %f %f\n", &x, &y, &z);
            if (res == EOF) {
                std::abort();
            }

            result.m_normals.push_back({ x, y, z });
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
            const auto res = fscanf(
                stream, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &iv1x, &iv1y, &iv1z, &iv2x, &iv2y, &iv2z, &iv3x, &iv3y, &iv3z);

            if (res == EOF) {
                std::abort();
            }

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

e172::Either<
    Mesh::Error,
    Mesh>
Mesh::parseObj(const std::string& data)
{
#ifdef _WIN32
    auto stream = e172vp::win_fmemopen(const_cast<char*>(data.data()), data.size(), "r");
#else
    auto stream = ::fmemopen(const_cast<char*>(data.data()), data.size(), "r");
#endif
    const auto result = parseObjFromCStream(stream);
    ::fclose(stream);
    return result;
}

e172::Either<
    Mesh::Error,
    Mesh>
Mesh::parseObj(const e172::ByteRange& data)
{
    return parseObj(std::string(data.begin(), data.end()));
}

e172::Either<
    Mesh::Error,
    Mesh>
Mesh::loadObj(const std::filesystem::path& path)
{
    auto stream = ::fopen(path.string().c_str(), "r");
    if (!stream) {
        return e172::Left(FileNotFound);
    }
    const auto result = parseObjFromCStream(stream);
    ::fclose(stream);
    return result;
}

}