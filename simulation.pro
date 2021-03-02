QT += core gui opengl

TARGET = simulation
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14 -mstackrealign
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    libs/glew-1.10.0/src/glew.c \
    src/main.cpp \
    src/mainwindow.cpp \
    src/view.cpp \
    src/viewformat.cpp \
    src/graphics/Shader.cpp \
    src/graphics/GraphicsDebug.cpp \
    src/simulation.cpp \
    src/graphics/shape.cpp \
    src/graphics/camera.cpp \
    src/graphics/MeshLoader.cpp

HEADERS += \
    libs/glew-1.10.0/include/GL/glew.h \
    src/mainwindow.h \
    src/view.h \
    src/viewformat.h \
    src/graphics/Shader.h \
    src/graphics/ShaderAttribLocations.h \
    src/graphics/GraphicsDebug.h \
    src/simulation.h \
    src/graphics/shape.h \
    src/graphics/camera.h \
    libs/tiny_obj/tiny_obj_loader.h \
    libs/unsupported/Eigen/OpenGLSupport \
    ui_mainwindow.h \
    src/graphics/MeshLoader.h

FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag

INCLUDEPATH += src libs glm libs/glew-1.10.0/include libs/Eigen/
DEPENDPATH += src libs glm libs/glew-1.10.0/include libs/Eigen/

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g
