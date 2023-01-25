cmake_minimum_required(VERSION 3.16)

# Sets project name
project(simulation LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specifies required Qt components
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Xml)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Allows you to include files from within those directories, without prefixing their filepaths
include_directories(src)
include_directories(libs)

add_definitions(-DGLM_FORCE_SWIZZLE)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    src/graphics/camera.cpp
    src/graphics/GraphicsDebug.cpp
    src/graphics/MeshLoader.cpp
    src/graphics/Shader.cpp
    src/graphics/shape.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/simulation.cpp
    src/view.cpp
    src/viewformat.cpp

    src/graphics/camera.h
    src/graphics/GraphicsDebug.h
    src/graphics/MeshLoader.h
    src/graphics/Shader.h
    src/graphics/shape.h
    src/mainwindow.h
    src/simulation.h
    src/view.h
    src/viewformat.h

    libs/tiny_obj/tiny_obj_loader.h
)

# GLEW: this creates its library and allows you to `#include "GL/glew.h"`
add_library(StaticGLEW STATIC libs/glew/src/glew.c)
include_directories(${PROJECT_NAME} PRIVATE libs/glew/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Concurrent
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
    Qt::Xml
    StaticGLEW
)

# This allows you to `#include "Eigen/..."`
target_include_directories(${PROJECT_NAME} PRIVATE
    Eigen
)

# Specifies other files
qt6_add_resources(${PROJECT_NAME} "Resources"
    PREFIX
        "/"
    FILES
        res/shaders/shader.frag
        res/shaders/shader.vert
)

# GLEW: this provides support for Windows (including 64-bit)
if (WIN32)
  add_compile_definitions(GLEW_STATIC)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32
    glu32
  )
endif()

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()

# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()

# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()

