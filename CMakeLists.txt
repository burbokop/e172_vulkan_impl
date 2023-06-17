cmake_minimum_required(VERSION 3.26.3)
project(e172_vulkan_impl LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

add_library(e172_vulkan_impl SHARED)

target_sources(e172_vulkan_impl
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.h
    ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.cpp
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/graphicsobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/graphicsobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/graphicsobjectcreateinfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/graphicsobjectcreateinfo.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/hardware.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/hardware.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/extensiontools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/extensiontools.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/validation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/validation.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/stringvector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/stringvector.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/presentationobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/presentationobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/vulkaninstancefactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/vulkaninstancefactory.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/logicdevicefactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/logicdevicefactory.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/renderpass.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/renderpass.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/commandpool.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/graphicsobject/commandpool.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertex.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/externaltexvertexobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/externaltexvertexobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/descriptorsetlayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/descriptorsetlayout.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/mesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/tools/mesh.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/pipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/pipeline.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/font.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/font.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/abstractvertexobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/abstractvertexobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/bootstrapobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/bootstrapobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/texturedvertexobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/texturedvertexobject.h
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/wirevertexobject.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/e172vp/vertexobjects/wirevertexobject.h)

find_package(Vulkan REQUIRED)


if($ENV{WITH_MAGURA})
    find_package(magura REQUIRED)
    target_connect_magura(e172_vulkan_impl)
    target_link_libraries(e172_vulkan_impl
        Vulkan::Vulkan
        freetype
        e172
        )
else($ENV{WITH_MAGURA})
    target_link_libraries(e172_vulkan_impl PRIVATE
        Vulkan::Vulkan
        SDL2
        freetype
        e172
        )
endif($ENV{WITH_MAGURA})


target_include_directories(e172_vulkan_impl PRIVATE $<BUILD_INTERFACE:/usr/include/freetype2>)
