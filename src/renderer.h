// Copyright 2023 Borys Boiko

#pragma once

#include <e172/graphics/abstractrenderer.h>
#include <e172/math/math.h>
#include <filesystem>
#include <list>
#include <map>
#include <string>

namespace e172vp {
class BootstrapObject;
class Mesh;
class AbstractVertexObject;
}

struct SDL_Surface;

namespace e172::impl::vulkan {

class GraphicsProvider;

class Renderer : public e172::AbstractRenderer {
    friend GraphicsProvider;
    struct Private {};

public:
    Renderer(
        Private,
        const std::string& title,
        const e172::Vector<uint32_t>& resolution,
        const std::map<
            std::string,
            std::filesystem::path>* fontPaths);
    ~Renderer();

    // AbstractRenderer interface
public:
    bool isValid() const;

    size_t presentEffectCount() const override;
    std::string presentEffectName(size_t index) const override;
    void drawEffect(
        size_t index,
        const e172::VariantVector& args) override;

    void fill(Color color) override;
    void drawPixel(
        const e172::Vector<double>& point,
        Color color) override;
    void drawLine(
        const e172::Vector<double>& point0,
        const e172::Vector<double>& point1,
        Color color) override;

    void drawRect(
        const e172::Vector<double>& point0,
        const e172::Vector<double>& point1,
        Color color,
        const e172::ShapeFormat& format = e172::ShapeFormat(false)) override;

    void drawSquare(
        const e172::Vector<double>& point,
        double radius,
        Color color) override;
    void drawCircle(
        const e172::Vector<double>& point,
        double radius,
        Color color) override;
    void drawDiagonalGrid(
        const e172::Vector<double>& point0,
        const e172::Vector<double>& point1,
        double interval,
        Color color) override;

    void drawImage(
        const e172::Image& image,
        const e172::Vector<double>& position,
        double angle,
        double zoom) override;

    e172::Vector<double> drawString(
        const std::string& string,
        const e172::Vector<double>& position,
        Color color,
        const e172::TextFormat& format) override;

    void applyLensEffect(
        const e172::Vector<double>& point0,
        const e172::Vector<double>& point1,
        double coefficient) override;

    void applySmooth(
        const e172::Vector<double>& point0,
        const e172::Vector<double>& point1,
        double coefficient) override;

    void enableEffect(std::uint64_t effect) override;
    void disableEffect(std::uint64_t effect) override;
    void setFullscreen(bool value) override;
    void setResolution(const e172::Vector<std::uint32_t>& value) override;
    e172::Vector<std::uint32_t> resolution() const override;
    void setDepth(int64_t depth) override;
    void modifyBitmap(const std::function<void(e172::Color*)>&) override
    {
        // TODO(burbokop): stuff
    }

    // AbstractRenderer interface
protected:
    bool update() override;

private:
    struct Reciept {
        bool modifyVertexBuffer = false;
        e172::Vector<double> position0;
        e172::Vector<double> position1;
        std::string text;
        float rotation = 0;
        Color color = 0;
        e172vp::Mesh* mesh = nullptr;
    };

    e172::Vector<double> transformedPosition(const e172::Vector<double>& position)
    {
        if (m_resolution.x() != 0 && m_resolution.y() != 0) {
            auto v = position * 2 - m_resolution.into<double>();
            return e172::Vector(v.x() / m_resolution.x(), v.y() / m_resolution.y());
        }
        return e172::Vector<double>();
    }

private:
    e172vp::BootstrapObject* m_bootstrapObject = nullptr;
    e172vp::Mesh* m_mesh0 = nullptr;
    e172vp::Mesh* m_mesh1 = nullptr;
    e172vp::Mesh* m_plateMesh = nullptr;
    e172vp::Mesh* m_lineMesh = nullptr;
    e172vp::Mesh* m_rectMesh = nullptr;
    e172vp::Mesh* m_circleMesh = nullptr;
    SDL_Surface* m_uiSurface = nullptr;
    const e172::Vector<std::uint32_t> m_resolution = e172::Vector<std::uint32_t>(800, 600);
    std::list<Reciept> m_reciepts;
    std::map<e172vp::Mesh*, std::list<e172vp::AbstractVertexObject*>> m_objectsPool;
};

} // namespace e172::impl::vulkan
