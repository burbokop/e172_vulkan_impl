// Copyright 2023 Borys Boiko

#pragma once

#include <e172/graphics/abstractgraphicsprovider.h>
#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace e172::impl::vulkan {

class GraphicsProvider : public e172::AbstractGraphicsProvider {
public:
    GraphicsProvider() = default;

    // AbstractGraphicsProvider interface
public:
    std::shared_ptr<AbstractRenderer> createRenderer(
        const std::string& title,
        const e172::Vector<uint32_t>& resolution) const override;

    e172::Image loadImage(const std::string& path) const override;
    e172::Image createImage(
        std::size_t width,
        std::size_t height) const override;
    void loadFont(
        const std::string& name,
        const std::filesystem::path& path) override;

    e172::Image createImage(
        std::size_t width,
        std::size_t height,
        const ImageInitFunction& imageInitFunction) const override;
    e172::Image createImage(
        std::size_t width,
        std::size_t height,
        const ImageInitFunctionExt& imageInitFunction) const override;
    bool fontLoaded(const std::string& name) const override;
    e172::Vector<std::uint32_t> screenSize() const override;

protected:
    void destructImage(e172::SharedContainer::DataPtr ptr) const override;
    e172::SharedContainer::Ptr imageBitMap(e172::SharedContainer::DataPtr ptr) const override;
    bool saveImage(
        e172::SharedContainer::DataPtr,
        const std::string&) const override;
    e172::SharedContainer::DataPtr imageFragment(
        e172::SharedContainer::DataPtr ptr,
        std::size_t x,
        std::size_t y,
        std::size_t& w,
        std::size_t& h) const override;

    e172::SharedContainer::DataPtr blitImages(
        e172::SharedContainer::DataPtr ptr0,
        e172::SharedContainer::DataPtr ptr1,
        std::ptrdiff_t x,
        std::ptrdiff_t y,
        std::size_t& w,
        std::size_t& h) const override;

    e172::SharedContainer::DataPtr transformImage(
        e172::SharedContainer::DataPtr,
        std::uint64_t) const override;

private:
    std::map<std::string, std::filesystem::path> m_fonts;
};

} // namespace e172::impl::vulkan
