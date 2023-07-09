#pragma once

#include <e172/graphics/abstractrenderer.h>
#include <e172/math/math.h>
#include <list>
#include <map>

namespace e172vp {
    class BootstrapObject;
    class Mesh;
    class AbstractVertexObject;
}

class SDL_Surface;

namespace e172::impl::vulkan {

class Renderer : public e172::AbstractRenderer
{
    e172vp::BootstrapObject *m_bootstrapObject = nullptr;
    e172vp::Mesh *m_mesh0 = nullptr;
    e172vp::Mesh *m_mesh1 = nullptr;
    e172vp::Mesh *m_plateMesh = nullptr;
    e172vp::Mesh *m_lineMesh = nullptr;
    e172vp::Mesh *m_rectMesh = nullptr;
    e172vp::Mesh *m_circleMesh = nullptr;
    SDL_Surface *uiSurface = nullptr;

    struct Reciept {
        bool modifyVertexBuffer = false;
        e172::Vector<double> position0;
        e172::Vector<double> position1;
        std::string text;
        float rotation;
        uint32_t color;
        e172vp::Mesh *mesh = nullptr;
    };

    const e172::Vector<double> m_resolution = e172::Vector<double>(800, 600);

    std::list<Reciept> m_reciepts;
    std::map<e172vp::Mesh*, std::list<e172vp::AbstractVertexObject*>> m_objectsPool;

    e172::Vector<double> transformedPosition(const e172::Vector<double> &position)
    {
        if(!e172::Math::cmpf(m_resolution.x(), 0) && !e172::Math::cmpf(m_resolution.y(), 0)) {
            auto v = position * 2 - m_resolution;
            return e172::Vector(v.x() / m_resolution.x(), v.y() / m_resolution.y());
        }
        return e172::Vector<double>();
    }

public:
    Renderer(const std::vector<std::string> &args);
    ~Renderer();
    // AbstractRenderer interface
protected:
    virtual bool update() override;

public:
    bool isValid() const;


    virtual size_t presentEffectCount() const override;
    virtual std::string presentEffectName(size_t index) const override;
    virtual void drawEffect(size_t index, const e172::VariantVector &args) override;

    virtual void fill(uint32_t color) override;
    virtual void drawPixel(const e172::Vector<double> &point, uint32_t color) override;
    virtual void drawLine(const e172::Vector<double> &point0,
                          const e172::Vector<double> &point1,
                          uint32_t color) override;

    virtual void drawRect(const e172::Vector<double> &point0,
                          const e172::Vector<double> &point1,
                          uint32_t color,
                          const e172::ShapeFormat &format = e172::ShapeFormat(false)) override;

    virtual void drawSquare(const e172::Vector<double> &point, int radius, uint32_t color) override;
    virtual void drawCircle(const e172::Vector<double> &point, int radius, uint32_t color) override;
    virtual void drawDiagonalGrid(const e172::Vector<double> &point0,
                                  const e172::Vector<double> &point1,
                                  int interval,
                                  uint32_t color) override;

    virtual void drawImage(const e172::Image &image,
                           const e172::Vector<double> &position,
                           double angle,
                           double zoom) override;

    virtual e172::Vector<double> drawString(const std::string &string,
                                            const e172::Vector<double> &position,
                                            uint32_t color,
                                            const e172::TextFormat &format) override;

    virtual void applyLensEffect(const e172::Vector<double> &point0,
                                 const e172::Vector<double> &point1,
                                 double coefficient) override;

    virtual void applySmooth(const e172::Vector<double> &point0,
                             const e172::Vector<double> &point1,
                             double coefficient) override;

    virtual void enableEffect(std::uint64_t effect) override;
    virtual void disableEffect(std::uint64_t effect) override;
    virtual void setFullscreen(bool value) override;
    virtual void setResolution(e172::Vector<double> value) override;
    virtual e172::Vector<double> resolution() const override;
    virtual e172::Vector<double> screenSize() const override;
    virtual void setDepth(int64_t depth) override;
    virtual void modifyBitmap(const std::function<void(e172::Color *)> &) override
    {
        // TODO
    }
};

} // namespace e172::impl::vulkan
