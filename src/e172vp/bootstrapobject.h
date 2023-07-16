#ifndef BOOTSTRAPOBJECT_H
#define BOOTSTRAPOBJECT_H

#include "font.h"
#include "presentationobject.h"
#include "vertexobjects/externaltexvertexobject.h"
#include "vertexobjects/wirevertexobject.h"
#include <e172/math/vector.h>

struct SDL_Window;

namespace e172vp {

class Pipeline;

class BootstrapObject {
public:
    BootstrapObject(const std::string &title,
                    const e172::Vector<uint32_t> &resolution,
                    const std::map<std::string, std::filesystem::path> *fontPaths);
    ~BootstrapObject();
    PresentationObject *presentationObject() const;

    ExternalTexVertexObject *addExternalTexVertexObject(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ExternalTexVertexObject *addExternalTexVertexObject(const Mesh &mesh);
    WireVertexObject *addWireVertexObject(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    void removeVertexObject(AbstractVertexObject *object);


    static std::vector<std::string> sdlExtensions(SDL_Window *window);

    static std::vector<char> readFile(const std::string &filename);

    bool isValid() const;

private:
    std::shared_ptr<Font> font(const std::string &name) const;

private:
    PresentationObject *m_presentationObject = nullptr;
    SDL_Window *m_window = nullptr;
    Pipeline *pipeline = nullptr;
    Pipeline *pipeline2 = nullptr;
    const std::map<std::string, std::filesystem::path> *m_fontPaths;
    mutable std::map<std::string, std::shared_ptr<Font>> m_fonts;
    bool m_isValid = true;
};
} // namespace e172vp
#endif // BOOTSTRAPOBJECT_H
