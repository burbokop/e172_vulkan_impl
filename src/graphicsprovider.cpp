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

e172::Image GraphicsProvider::loadImage(const std::string &) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(int, int) const
{
    return e172::Image();
}

void GraphicsProvider::loadFont(const std::string &, const std::string &) {}

e172::Image GraphicsProvider::createImage(
    int, int, const e172::AbstractGraphicsProvider::ImageInitFunction &) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(
    int, int, const e172::AbstractGraphicsProvider::ImageInitFunctionExt &) const
{
    return e172::Image();
}

bool GraphicsProvider::fontLoaded(const std::string &) const
{
    return false;
}

void GraphicsProvider::destructImage(e172::SharedContainer::DataPtr) const {}

e172::SharedContainer::Ptr GraphicsProvider::imageBitMap(e172::SharedContainer::DataPtr) const
{
    return nullptr;
}

bool GraphicsProvider::saveImage(e172::SharedContainer::DataPtr, const std::string &) const
{
    return false;
}

e172::SharedContainer::DataPtr GraphicsProvider::imageFragment(
    e172::SharedContainer::DataPtr, std::size_t, std::size_t, std::size_t &, std::size_t &) const
{
    return nullptr;
}

e172::SharedContainer::DataPtr GraphicsProvider::blitImages(e172::SharedContainer::DataPtr,
                                                            e172::SharedContainer::DataPtr,
                                                            int,
                                                            int,
                                                            std::size_t &,
                                                            std::size_t &) const
{
    return nullptr;
}

e172::SharedContainer::DataPtr GraphicsProvider::transformImage(e172::SharedContainer::DataPtr,
                                                                uint64_t) const
{
    return nullptr;
}

} // namespace e172::impl::vulkan
