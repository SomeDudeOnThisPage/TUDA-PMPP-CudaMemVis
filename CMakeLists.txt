cmake_minimum_required(VERSION 3.0)
project(ExVis)
set(CMAKE_CXX_STANDARD 17)

# setup base paths
set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXVIS_CLIENT_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)

# setup GLFW
set(GLFW_DIRECTORY ${EXVIS_CLIENT_LIBRARY_DIR}/glfw-3.3-stable)
# -DGLFW_DIRECTORY="lib/glfw-3.3.4"
# if (NOT DEFINED GLFW_DIRECTORY)
#     message( FATAL_ERROR "GLFW_DIRECTORY is not set. You must download and unpack GLFW and add -DGLFW_DIRECTORY=[ relative path to your glfw directory ] as a cmake parameter." )
# endif()

set(GLFW_BUILD_EXAMPLES FALSE)
set(GLFW_BUILD_TESTS FALSE)
set(GLFW_BUILD_DOCS FALSE)

set(GLFW_LIBRARY_INCLUDE ${GLFW_DIRECTORY}/include)
set(GLFW_DEPENDENCIES ${GLFW_DIRECTORY}/deps)
add_subdirectory(${GLFW_DIRECTORY})
include_directories(
    ${GLFW_LIBRARY_INCLUDE}
    ${GLFW_DEPENDENCIES}
    ${VULKAN_SDK_INCLUDE}
)

include_directories(${EXVIS_CLIENT_LIBRARY_DIR}/glad-ogl-3.0/include)

include_directories(${EXVIS_CLIENT_LIBRARY_DIR}/imgui-1.85-opengl3-glfw)
include(${EXVIS_CLIENT_LIBRARY_DIR}/imgui-1.85-opengl3-glfw/imgui-cmake.txt)

set(EXVIS_CLIENT_SOURCE
    src/main.cpp
    src/exvis.h
    src/platform/GLAsset.cpp
    src/platform/GLTexture.cpp
    src/platform/GLShader.cpp
    src/platform/GLBindable.h
    src/platform/GLFrameBuffer.cpp
    src/platform/TraceFile.cpp
    src/platform/window.cpp
    src/gui/MemoryBlock.cpp
    src/gui/GUIElement.cpp
    lib/glad-ogl-3.0/src/glad.c
)

add_executable(ExVis ${EXVIS_CLIENT_SOURCE} ${IMGUI_SOURCE})
target_link_libraries(ExVis glfw ${GLFW_LIBRARIES} ${VULKAN_SDK_LINK_LIBRARY})
target_compile_definitions(ExVis PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")