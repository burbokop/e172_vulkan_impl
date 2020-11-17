# vulkan_implementation
Library implements e172 io api using Vulkan SDK (this project is incompleate).

## adding vulkan_implementation to your c++ project
```
git submodule add https://github.com/burbokop/vulkan_implementation.git
git submodule add https://github.com/burbokop/e172.git (because vulkan_implementation depends on e172 lib)
```
in `CMakeLists.txt`:

include(${CMAKE_CURRENT_LIST_DIR}/vulkan_implementation/vulkan_implementation.cmake)
target_link_libraries(your_executable_or_lib
    vulkan_impl
)
