#include "graphicsprovider.h"

#include "renderer.h"
#include <SDL2/SDL_video.h>

namespace e172::impl::vulkan {

std::shared_ptr<AbstractRenderer> GraphicsProvider::createRenderer(
    const std::string &title, const e172::Vector<uint32_t> &resolution) const
{
    const auto renderer = std::make_shared<Renderer>(Renderer::Private{},
                                                     title,
                                                     resolution,
                                                     &m_fonts);
    installParentToRenderer(*renderer);
    return renderer;
}

e172::Image GraphicsProvider::loadImage(const std::string &) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(std::size_t, std::size_t) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(
    std::size_t, std::size_t, const e172::AbstractGraphicsProvider::ImageInitFunction &) const
{
    return e172::Image();
}

e172::Image GraphicsProvider::createImage(
    std::size_t, std::size_t, const e172::AbstractGraphicsProvider::ImageInitFunctionExt &) const
{
    return e172::Image();
}

void GraphicsProvider::loadFont(const std::string &name, const std::filesystem::path &path)
{
    m_fonts[name] = path;
}

bool GraphicsProvider::fontLoaded(const std::string &name) const
{
    return m_fonts.find(name) != m_fonts.end();
}

e172::Vector<uint32_t> GraphicsProvider::screenSize() const
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    assert(displayMode.w >= 0);
    assert(displayMode.h >= 0);
    return e172::Vector<std::uint32_t>(displayMode.w, displayMode.h);
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
                                                            std::ptrdiff_t,
                                                            std::ptrdiff_t,
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
