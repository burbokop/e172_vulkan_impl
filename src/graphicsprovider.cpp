#include "graphicsprovider.h"

namespace e172::impl::vulkan {

GraphicsProvider::GraphicsProvider(const std::vector<std::string> &args)
    : e172::AbstractGraphicsProvider(args)
{
    m_renderer = new Renderer(args);
    installParentToRenderer(m_renderer);
}

GraphicsProvider::~GraphicsProvider()
{
    delete m_renderer;
}

e172::AbstractRenderer *GraphicsProvider::renderer() const
{
    return m_renderer;
}

bool GraphicsProvider::isValid() const
{
    return m_renderer->isValid();
}

e172::Image GraphicsProvider::loadImage(const std::string &path) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(int width, int height) const
{
    return e172::Image();
}

void GraphicsProvider::loadFont(const std::string &name, const std::string &path) {}

e172::Image GraphicsProvider::createImage(
    int width,
    int height,
    const e172::AbstractGraphicsProvider::ImageInitFunction &imageInitFunction) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(
    int width,
    int height,
    const e172::AbstractGraphicsProvider::ImageInitFunctionExt &imageInitFunction) const
{
    return e172::Image();
}

bool GraphicsProvider::fontLoaded(const std::string &name) const
{
    return false;
}

void GraphicsProvider::destructImage(e172::SharedContainer::DataPtr ptr) const {}

e172::SharedContainer::Ptr GraphicsProvider::imageBitMap(e172::SharedContainer::DataPtr ptr) const
{
    return nullptr;
}

bool GraphicsProvider::saveImage(e172::SharedContainer::DataPtr, const std::string &) const
{
    return false;
}

e172::SharedContainer::DataPtr GraphicsProvider::imageFragment(e172::SharedContainer::DataPtr ptr,
                                                               std::size_t x,
                                                               std::size_t y,
                                                               std::size_t &w,
                                                               std::size_t &h) const
{
    return nullptr;
}

e172::SharedContainer::DataPtr GraphicsProvider::blitImages(e172::SharedContainer::DataPtr ptr0,
                                                            e172::SharedContainer::DataPtr ptr1,
                                                            int x,
                                                            int y,
                                                            std::size_t &w,
                                                            std::size_t &h) const
{
    return nullptr;
}

e172::SharedContainer::DataPtr GraphicsProvider::transformImage(e172::SharedContainer::DataPtr,
                                                                uint64_t) const
{
    return nullptr;
}

} // namespace e172::impl::vulkan
