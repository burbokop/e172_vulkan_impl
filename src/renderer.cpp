#include "renderer.h"

#include "e172vp/bootstrapobject.h"
#include "e172vp/tools/mesh.h"
#include <SDL2/SDL_image.h>
#include <e172/additional.h>
#include <e172/utility/resource.h>
#include <glm/gtc/matrix_transform.hpp>

const e172::ByteRange ship1MeshResource = e172_load_resource(resources_meshes_ship1_obj);
const e172::ByteRange ship2MeshResource = e172_load_resource(resources_meshes_ship2_obj);
const e172::ByteRange bananaImageResource = e172_load_resource(resources_images_banana_png);

namespace e172::impl::vulkan {

Renderer::Renderer(Private,
                   const std::string &title,
                   const e172::Vector<uint32_t> &resolution,
                   const std::map<std::string, std::filesystem::path> *fontPaths)
{
    m_bootstrapObject = new e172vp::BootstrapObject(title, resolution, fontPaths);

    m_mesh0 = new e172vp::Mesh(e172vp::Mesh::parseObj(ship1MeshResource).unwrap());
    m_mesh1 = new e172vp::Mesh(e172vp::Mesh::parseObj(ship2MeshResource).unwrap());
    m_plateMesh = new e172vp::Mesh(e172vp::Mesh::plate(0.01));

    m_lineMesh = new e172vp::Mesh();
    m_lineMesh->m_vertices = {{}, {}};
    m_lineMesh->m_vertexIndices = {0, 1};
    m_lineMesh->m_hasTexture = false;

    m_rectMesh = new e172vp::Mesh();
    m_rectMesh->m_vertices = {{}, {}, {}, {}};
    m_rectMesh->m_vertexIndices = {0, 1, 2, 2, 3, 0};
    m_rectMesh->m_hasTexture = false;

    m_circleMesh = new e172vp::Mesh();
    m_circleMesh->m_vertices = {{}, {}, {}, {}};
    m_circleMesh->m_vertexIndices = {0, 1, 2, 2, 3, 0};
    m_circleMesh->m_hasTexture = false;

    //m_lineMesh = new e172vp::Mesh();
    //m_lineMesh->vertices = {
    //    {  { tp0.float32X(), tp0.float32Y(), 0 }, { 0., 1., 0. }, {} },
    //    {  { tp1.float32X(), tp1.float32Y(), 0 }, { 0., 1., 0. }, {} }
    //};
    //m_lineMesh->vertexIndices = {
    //    0, 1
    //};

    //bananaImageResource;

    m_uiSurface = IMG_LoadPNG_RW(
        SDL_RWFromConstMem(bananaImageResource.begin(), bananaImageResource.size()));
}

Renderer::~Renderer()
{
    if(m_bootstrapObject)
        delete m_bootstrapObject;
}

bool Renderer::update()
{
    if(m_bootstrapObject) {
        //GET OBJECTS FROM POOL
        std::map<e172vp::Mesh*, std::list<e172vp::AbstractVertexObject*>> usedObjects;
        for (const auto &reciept : m_reciepts) {
            auto& list = m_objectsPool[reciept.mesh];

            e172vp::AbstractVertexObject *obj = nullptr;
            if(list.size() > 0) {
                obj = list.front();
                list.remove(obj);
            } else {
                if (reciept.mesh->hasTexture()) {
                    obj = m_bootstrapObject->addExternalTexVertexObject(*reciept.mesh);
                } else {
                    obj = m_bootstrapObject->addWireVertexObject(e172vp::Vertex::fromGlm(
                                                                     reciept.mesh->vertices()),
                                                                 reciept.mesh->vertexIndices());
                }
                if(!reciept.modifyVertexBuffer) {
                    if(auto extTexObj = dynamic_cast<e172vp::ExternalTexVertexObject*>(obj)) {
                        extTexObj->setScale(glm::scale(glm::mat4(1.), glm::vec3(0.02)));
                    }
                }
            }

            if(auto extTexObj = dynamic_cast<e172vp::ExternalTexVertexObject*>(obj)) {
                extTexObj->setRotation(glm::rotate(glm::mat4(1.), reciept.rotation, glm::vec3(0., 0., 1.)));
            }
            if(reciept.modifyVertexBuffer) {
                if(auto wireObj = dynamic_cast<e172vp::WireVertexObject*>(obj)) {
                    glm::vec3 color = {
                        static_cast<uint8_t>(reciept.color >> 16) / 256.,
                        static_cast<uint8_t>(reciept.color >> 8) / 256.,
                        static_cast<uint8_t>(reciept.color >> 0) / 256.
                    };

                    if(reciept.mesh == m_lineMesh) {

                        wireObj->setVertices({
                                             { { reciept.position0.float32X(), reciept.position0.float32Y(), 0 }, color, {} },
                                             { { reciept.position1.float32X(), reciept.position1.float32Y(), 0 }, color, {} }
                                         });
                    } else if(reciept.mesh == m_rectMesh) {
                        wireObj->setVertices({
                                             { { reciept.position0.float32X(), reciept.position0.float32Y(), 0 }, color, {} },
                                             { { reciept.position1.float32X(), reciept.position0.float32Y(), 0 }, color, {} },
                                             { { reciept.position1.float32X(), reciept.position1.float32Y(), 0 }, color, {} },
                                             { { reciept.position0.float32X(), reciept.position1.float32Y(), 0 }, color, {} }
                                         });
                    } else if(reciept.mesh == m_circleMesh) {
                        glm::vec3 p0((reciept.position0.float32X() + reciept.position1.float32X()) / 2, reciept.position0.float32Y(), 0);
                        glm::vec3 p1(reciept.position1.float32X(), (reciept.position0.float32Y() + reciept.position1.float32Y()) / 2, 0);
                        glm::vec3 p2((reciept.position0.float32X() + reciept.position1.float32X()) / 2, reciept.position1.float32Y(), 0);
                        glm::vec3 p3(reciept.position0.float32X(), (reciept.position0.float32Y() + reciept.position1.float32Y()) / 2, 0);

                        wireObj->setVertices({
                                             { p0, color, {} },
                                             { p1, color, {} },
                                             { p2, color, {} },
                                             { p3, color, {} }
                                         });
                    } else if(reciept.mesh == m_plateMesh) {
                        const auto plate = e172vp::Mesh::plate({ reciept.position0.float32X(), reciept.position0.float32Y() }, { reciept.position1.float32X(), reciept.position1.float32Y() });
                        wireObj->setVertices(
                            e172vp::Vertex::fromGlm(plate.vertices(), plate.uvMap()));
                    }
                }
            } else {
                if(auto extTexObj = dynamic_cast<e172vp::ExternalTexVertexObject*>(obj)) {
                    extTexObj->setTranslation(glm::translate(glm::mat4(1.), glm::vec3(reciept.position0.float32X(), reciept.position0.float32Y(), 0)));
                }
            }
            assert(obj);
            if (obj) {
                obj->setVisible(true);
            }

            usedObjects[reciept.mesh].push_back(obj);
        }

        //HIDE OBJECTS IN POOL
        for (const auto &l : m_objectsPool) {
            for(auto o : l.second)
                o->setVisible(false);
        }

        //std::cout << "ptr: " << uiSurface << "\n";
        //m_bootstrapObject->presentationObject()->updateUiImage(uiSurface->w, uiSurface->h, uiSurface->pixels);


        //PRESENT
        m_reciepts.clear();
        m_bootstrapObject->presentationObject()->present();

        //RETURN OBJECTS TO POOL
        for(const auto& list : usedObjects) {
            for(auto object : list.second) {
                m_objectsPool[list.first].push_back(object);
            }
        }

        return true;
    }
    return false;
}

bool Renderer::isValid() const
{
    if(m_bootstrapObject)
        return m_bootstrapObject->isValid();

    return false;
}

size_t Renderer::presentEffectCount() const
{
    return 0;
}

std::string Renderer::presentEffectName(std::size_t) const
{
    return "";
}

void Renderer::drawEffect(std::size_t, const e172::VariantVector &) {}

void Renderer::fill(Color) {}

void Renderer::drawPixel(const e172::Vector<double> &point, Color color)
{
    drawSquare(point, 1, color);
}

void Renderer::drawLine(const e172::Vector<double> &point0,
                        const e172::Vector<double> &point1,
                        Color color)
{
    Reciept reciept;
    reciept.position0 = transformedPosition(point0);
    reciept.position1 = transformedPosition(point1);
    reciept.mesh = m_lineMesh;
    reciept.modifyVertexBuffer = true;
    reciept.color = color;
    m_reciepts.push_back(reciept);
}

void Renderer::drawRect(const e172::Vector<double> &point0,
                        const e172::Vector<double> &point1,
                        Color color,
                        const e172::ShapeFormat &)
{
    Reciept reciept;
    reciept.position0 = transformedPosition(point0);
    reciept.position1 = transformedPosition(point1);
    reciept.mesh = m_rectMesh;
    reciept.modifyVertexBuffer = true;
    reciept.color = color;
    m_reciepts.push_back(reciept);
}

void Renderer::drawSquare(const e172::Vector<double> &point, double radius, Color color)
{
    Reciept reciept;
    reciept.position0 = transformedPosition(point - radius);
    reciept.position1 = transformedPosition(point + radius);
    reciept.mesh = m_rectMesh;
    reciept.modifyVertexBuffer = true;
    reciept.color = color;
    m_reciepts.push_back(reciept);
}

void Renderer::drawCircle(const e172::Vector<double> &point, double radius, Color color)
{
    Reciept reciept;
    reciept.position0 = transformedPosition(point - radius);
    reciept.position1 = transformedPosition(point + radius);
    reciept.mesh = m_circleMesh;
    reciept.modifyVertexBuffer = true;
    reciept.color = color;
    m_reciepts.push_back(reciept);
}

void Renderer::drawDiagonalGrid(const e172::Vector<double> &point0,
                                const e172::Vector<double> &point1,
                                double interval,
                                Color color)
{
    (void)point0;
    (void)point1;
    (void)interval;
    (void)color;
}

void Renderer::drawImage(const e172::Image &image,
                         const e172::Vector<double> &position,
                         double angle,
                         double zoom)
{
    Reciept reciept;
    reciept.position0 = transformedPosition(position);
    reciept.rotation = angle;
    reciept.mesh = m_mesh0;
    m_reciepts.push_back(reciept);
    (void)image;
    (void)zoom;
}

e172::Vector<double> Renderer::drawString(const std::string &string,
                                          const e172::Vector<double> &position,
                                          Color color,
                                          const e172::TextFormat &format)
{
    e172::Vector<double> size(string.size() * 8, 8);

    Reciept reciept;
    reciept.position0 = transformedPosition(position);
    reciept.position1 = transformedPosition(position + size);
    reciept.mesh = m_plateMesh;
    reciept.text = string;
    reciept.modifyVertexBuffer = true;
    reciept.color = color;
    reciept.rotation = 0;
    m_reciepts.push_back(reciept);

    (void)format;
    return size;
}

void Renderer::applyLensEffect(const e172::Vector<double> &point0,
                               const e172::Vector<double> &point1,
                               double coefficient)
{
    (void)point0;
    (void)point1;
    (void)coefficient;
}

void Renderer::applySmooth(const e172::Vector<double> &point0,
                           const e172::Vector<double> &point1,
                           double coefficient)
{
    (void)point0;
    (void)point1;
    (void)coefficient;
}

void Renderer::enableEffect(uint64_t effect)
{
    (void)effect;
}

void Renderer::disableEffect(uint64_t effect)
{
    (void)effect;
}

void Renderer::setFullscreen(bool value)
{
    (void)value;
}

void Renderer::setResolution(const e172::Vector<uint32_t> &value)
{
    (void)value;
}

e172::Vector<uint32_t> Renderer::resolution() const
{
    return m_resolution;
}

void Renderer::setDepth(int64_t) {}

} // namespace e172::impl::vulkan
