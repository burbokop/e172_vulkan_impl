cmake_minimum_required(VERSION 3.26.3)

project(
  e172_vulkan_impl
  LANGUAGES CXX
  VERSION 0.0.1
  DESCRIPTION "Vulkan implementation of e172"
  HOMEPAGE_URL "https://github.com/burbokop/e172_vulkan_impl")

option(ENABLE_FIND_E172_PACKAGE
       "Find e172 package (searches for link and include directories if OFF)"
       ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

include(GNUInstallDirs)

add_library(e172_vulkan_impl SHARED)

set(INSTALL_NAMESPACE e172)
set(INSTALL_IMPL_NAMESPACE vulkan)

set(INSTALLDIR
    "${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAMESPACE}/impl/${INSTALL_IMPL_NAMESPACE}"
)

target_sources(
  e172_vulkan_impl
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/graphicsprovider.h>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src/renderer.h>
         $<INSTALL_INTERFACE:${INSTALLDIR}/renderer.h>
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/graphicsprovider.cpp
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

if(ENABLE_FIND_E172_PACKAGE)
  find_package(e172 REQUIRED)
endif()

if($ENV{WITH_MAGURA})
  find_package(magura REQUIRED)
  target_connect_magura(e172_vulkan_impl)
  target_link_libraries(e172_vulkan_impl Vulkan::Vulkan freetype)
else($ENV{WITH_MAGURA})
  target_link_libraries(e172_vulkan_impl PRIVATE Vulkan::Vulkan SDL2 freetype)
endif($ENV{WITH_MAGURA})

if(ENABLE_FIND_E172_PACKAGE)
  target_link_libraries(e172_sdl_impl e172::e172)
else()
  target_link_libraries(e172_sdl_impl e172)
endif()

target_include_directories(e172_vulkan_impl
                           PRIVATE $<BUILD_INTERFACE:/usr/include/freetype2>)

if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL
                                                "GNU"))
  target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/src/"
  DESTINATION
    "${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAMESPACE}/impl/${INSTALL_IMPL_NAMESPACE}"
  FILES_MATCHING
  PATTERN "*.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(
  EXPORT ${PROJECT_NAME}_targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${INSTALL_NAMESPACE}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
