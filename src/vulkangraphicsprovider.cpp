#include "vulkangraphicsprovider.h"

VulkanGraphicsProvider::VulkanGraphicsProvider(const std::vector<std::string> &args) : e172::AbstractGraphicsProvider(args) {
    m_renderer = new VulkanRenderer(args);
    installParentToRenderer(m_renderer);
}

VulkanGraphicsProvider::~VulkanGraphicsProvider() {
    delete m_renderer;
}

e172::AbstractRenderer *VulkanGraphicsProvider::renderer() const {
    return m_renderer;
}

bool VulkanGraphicsProvider::isValid() const {
    return m_renderer->isValid();
}

e172::Image VulkanGraphicsProvider::loadImage(const std::string &path) const {
    return e172::Image();
}

e172::Image VulkanGraphicsProvider::createImage(int width, int height) const {
    return e172::Image();
}

void VulkanGraphicsProvider::loadFont(const std::string &name, const std::string &path) {

}

e172::Image VulkanGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunction &imageInitFunction) const {
    return e172::Image();
}

e172::Image VulkanGraphicsProvider::createImage(int width, int height, const e172::AbstractGraphicsProvider::ImageInitFunctionExt &imageInitFunction) const {
    return e172::Image();
}

bool VulkanGraphicsProvider::fontLoaded(const std::string &name) const {
    return false;
}

void VulkanGraphicsProvider::destructImage(e172::SharedContainer::data_ptr ptr) const {
}

e172::SharedContainer::ptr VulkanGraphicsProvider::imageBitMap(e172::SharedContainer::data_ptr ptr) const {
    return nullptr;
}

bool VulkanGraphicsProvider::saveImage(e172::SharedContainer::data_ptr, const std::string &) const {
    return false;
}

e172::SharedContainer::data_ptr VulkanGraphicsProvider::imageFragment(e172::SharedContainer::data_ptr ptr, int x, int y, int &w, int &h) const {
    return nullptr;
}

e172::SharedContainer::data_ptr VulkanGraphicsProvider::blitImages(e172::SharedContainer::data_ptr ptr0, e172::SharedContainer::data_ptr ptr1, int x, int y, int &w, int &h) const {
    return nullptr;
}

e172::SharedContainer::data_ptr VulkanGraphicsProvider::transformImage(e172::SharedContainer::data_ptr, uint64_t) const {
    return nullptr;
}
