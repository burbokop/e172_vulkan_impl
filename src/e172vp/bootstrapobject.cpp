#include "bootstrapobject.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_vulkan.h>
#include <e172/additional.h>
#include <e172/debug.h>
#include <e172/utility/buffer.h>
#include <e172/utility/resource.h>
#include <fstream>

const e172::ByteRange vertUniformResource = e172_load_resource(resources_shaders_vert_uniform_spv);
const e172::ByteRange fragSamplerResource = e172_load_resource(resources_shaders_frag_sampler_spv);
const e172::ByteRange vertLinestripResource = e172_load_resource(
    resources_shaders_vert_linestrip_spv);
const e172::ByteRange fragInterResource = e172_load_resource(resources_shaders_frag_inter_spv);

e172vp::PresentationObject *e172vp::BootstrapObject::presentationObject() const
{
    return m_presentationObject;
}

e172vp::ExternalTexVertexObject *e172vp::BootstrapObject::addExternalTexVertexObject(const std::vector<e172vp::Vertex> &vertices, const std::vector<uint32_t> &indices) {
    if (const auto f = font("")) {
        auto obj = m_presentationObject
                       ->addVertexObject<ExternalTexVertexObject>(vertices,
                                                                  indices,
                                                                  f->character('G').imageView());
        obj->setPipeline(pipeline);
        obj->setBindGlobalDescriptorSet(true);
        return obj;
    } else {
        e172::Debug::warning("Can not find default font");
        return nullptr;
    }
}

e172vp::ExternalTexVertexObject *e172vp::BootstrapObject::addExternalTexVertexObject(
    const e172vp::Mesh &mesh)
{
    if (const auto f = font("")) {
        auto obj = m_presentationObject
                       ->addVertexObject<ExternalTexVertexObject>(Vertex::fromGlm(mesh.vertices(),
                                                                                  mesh.uvMap()),
                                                                  mesh.vertexIndices(),
                                                                  f->character('G').imageView());
        obj->setPipeline(pipeline);
        obj->setBindGlobalDescriptorSet(true);
        return obj;
    } else {
        e172::Debug::warning("Can not find default font");
        return nullptr;
    }
}

e172vp::WireVertexObject *e172vp::BootstrapObject::addWireVertexObject(const std::vector<e172vp::Vertex> &vertices, const std::vector<uint32_t> &indices) {
    auto obj = m_presentationObject->addVertexObject<WireVertexObject>(vertices, indices);
    obj->setPipeline(pipeline2);
    return obj;
}

void e172vp::BootstrapObject::removeVertexObject(e172vp::AbstractVertexObject *object) {
    m_presentationObject->removeVertexObject(object);
}

std::vector<std::string> e172vp::BootstrapObject::sdlExtensions(SDL_Window *window) {
    uint32_t extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char *> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensionNames.data());

    std::vector<std::string> result(extensionCount);
    for(uint32_t i = 0; i < extensionCount; ++i) {
        result[i] = extensionNames[i];
    }
    return result;
}

std::vector<char> e172vp::BootstrapObject::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

bool e172vp::BootstrapObject::isValid() const {
    return m_isValid;
}

std::shared_ptr<e172vp::Font> e172vp::BootstrapObject::font(const std::string &name) const
{
    auto it = m_fonts.find(name);
    if (it == m_fonts.end()) {
        const auto pathIt = m_fontPaths->find(name);
        if (pathIt != m_fontPaths->end()) {
            it = m_fonts.insert(
                it,
                {name,
                 std::make_shared<Font>(m_presentationObject->graphicsObject()->logicalDevice(),
                                        m_presentationObject->graphicsObject()->physicalDevice(),
                                        m_presentationObject->graphicsObject()->commandPool(),
                                        m_presentationObject->graphicsObject()->graphicsQueue(),
                                        pathIt->second,
                                        128)});
        }
    }
    if (it != m_fonts.end()) {
        return it->second;
    }
    return nullptr;
}

static std::vector<char> resourceToCharVec(const e172::ByteRange &range)
{
    return std::vector<char>(reinterpret_cast<const char *>(range.begin()),
                             reinterpret_cast<const char *>(range.end()));
}

e172vp::BootstrapObject::BootstrapObject(
    const std::string &title,
    const e172::Vector<uint32_t> &resolution,
    const std::map<std::string, std::filesystem::path> *fontPaths)
    : m_fontPaths(fontPaths)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    m_window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                resolution.x(),
                                resolution.y(),
                                SDL_WINDOW_VULKAN);

    GraphicsObjectCreateInfo graphicsObjectCreateInfo;
    graphicsObjectCreateInfo.setRequiredExtensions(sdlExtensions(m_window));
    graphicsObjectCreateInfo.setApplicationName("test-app");
    graphicsObjectCreateInfo.setApplicationVersion(1);
    graphicsObjectCreateInfo.setDebugEnabled(true);
    graphicsObjectCreateInfo.setRequiredDeviceExtensions({ VK_KHR_SWAPCHAIN_EXTENSION_NAME });
    graphicsObjectCreateInfo.setSubpassCount(2);
    graphicsObjectCreateInfo.setSurfaceCreator([this](vk::Instance i, vk::SurfaceKHR *s) {
        VkSurfaceKHR ss;
        SDL_Vulkan_CreateSurface(m_window, i, &ss);
        *s = ss;
    });

    m_presentationObject = new PresentationObject(graphicsObjectCreateInfo);

    pipeline = m_presentationObject->createPipeline(0,
                                                    PresentationObject::GlobalDSL
                                                        | PresentationObject::SamplerDSL
                                                        | PresentationObject::PerObjectDSL,
                                                    resourceToCharVec(vertUniformResource),
                                                    resourceToCharVec(fragSamplerResource),
                                                    vk::PrimitiveTopology::eTriangleList);

    pipeline2 = m_presentationObject->createPipeline(1,
                                                     PresentationObject::GlobalDSL,
                                                     resourceToCharVec(vertLinestripResource),
                                                     resourceToCharVec(fragInterResource),
                                                     vk::PrimitiveTopology::eLineStrip);

    if (const auto f = font("")) {
        m_presentationObject->setUiImage(f->character('a').image());
    } else {
        e172::Debug::warning("Can not find default font");
    }
}

e172vp::BootstrapObject::~BootstrapObject() {
    delete m_presentationObject;
}
