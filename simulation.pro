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
    src/graphics/camera.cpp

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
    libs/Eigen/src/Core/arch/AltiVec/Complex.h \
    libs/Eigen/src/Core/arch/AltiVec/MathFunctions.h \
    libs/Eigen/src/Core/arch/AltiVec/PacketMath.h \
    libs/Eigen/src/Core/arch/AVX/Complex.h \
    libs/Eigen/src/Core/arch/AVX/MathFunctions.h \
    libs/Eigen/src/Core/arch/AVX/PacketMath.h \
    libs/Eigen/src/Core/arch/AVX/TypeCasting.h \
    libs/Eigen/src/Core/arch/AVX512/MathFunctions.h \
    libs/Eigen/src/Core/arch/AVX512/PacketMath.h \
    libs/Eigen/src/Core/arch/CUDA/Complex.h \
    libs/Eigen/src/Core/arch/CUDA/Half.h \
    libs/Eigen/src/Core/arch/CUDA/MathFunctions.h \
    libs/Eigen/src/Core/arch/CUDA/PacketMath.h \
    libs/Eigen/src/Core/arch/CUDA/PacketMathHalf.h \
    libs/Eigen/src/Core/arch/CUDA/TypeCasting.h \
    libs/Eigen/src/Core/arch/Default/Settings.h \
    libs/Eigen/src/Core/arch/NEON/Complex.h \
    libs/Eigen/src/Core/arch/NEON/MathFunctions.h \
    libs/Eigen/src/Core/arch/NEON/PacketMath.h \
    libs/Eigen/src/Core/arch/SSE/Complex.h \
    libs/Eigen/src/Core/arch/SSE/MathFunctions.h \
    libs/Eigen/src/Core/arch/SSE/PacketMath.h \
    libs/Eigen/src/Core/arch/SSE/TypeCasting.h \
    libs/Eigen/src/Core/arch/ZVector/Complex.h \
    libs/Eigen/src/Core/arch/ZVector/MathFunctions.h \
    libs/Eigen/src/Core/arch/ZVector/PacketMath.h \
    libs/Eigen/src/Core/functors/AssignmentFunctors.h \
    libs/Eigen/src/Core/functors/BinaryFunctors.h \
    libs/Eigen/src/Core/functors/NullaryFunctors.h \
    libs/Eigen/src/Core/functors/StlFunctors.h \
    libs/Eigen/src/Core/functors/TernaryFunctors.h \
    libs/Eigen/src/Core/functors/UnaryFunctors.h \
    libs/Eigen/src/Core/products/GeneralBlockPanelKernel.h \
    libs/Eigen/src/Core/products/GeneralMatrixMatrix.h \
    libs/Eigen/src/Core/products/GeneralMatrixMatrix_BLAS.h \
    libs/Eigen/src/Core/products/GeneralMatrixMatrixTriangular.h \
    libs/Eigen/src/Core/products/GeneralMatrixMatrixTriangular_BLAS.h \
    libs/Eigen/src/Core/products/GeneralMatrixVector.h \
    libs/Eigen/src/Core/products/GeneralMatrixVector_BLAS.h \
    libs/Eigen/src/Core/products/Parallelizer.h \
    libs/Eigen/src/Core/products/SelfadjointMatrixMatrix.h \
    libs/Eigen/src/Core/products/SelfadjointMatrixMatrix_BLAS.h \
    libs/Eigen/src/Core/products/SelfadjointMatrixVector.h \
    libs/Eigen/src/Core/products/SelfadjointMatrixVector_BLAS.h \
    libs/Eigen/src/Core/products/SelfadjointProduct.h \
    libs/Eigen/src/Core/products/SelfadjointRank2Update.h \
    libs/Eigen/src/Core/products/TriangularMatrixMatrix.h \
    libs/Eigen/src/Core/products/TriangularMatrixMatrix_BLAS.h \
    libs/Eigen/src/Core/products/TriangularMatrixVector.h \
    libs/Eigen/src/Core/products/TriangularMatrixVector_BLAS.h \
    libs/Eigen/src/Core/products/TriangularSolverMatrix.h \
    libs/Eigen/src/Core/products/TriangularSolverMatrix_BLAS.h \
    libs/Eigen/src/Core/products/TriangularSolverVector.h \
    libs/Eigen/src/Core/util/BlasUtil.h \
    libs/Eigen/src/Core/util/Constants.h \
    libs/Eigen/src/Core/util/DisableStupidWarnings.h \
    libs/Eigen/src/Core/util/ForwardDeclarations.h \
    libs/Eigen/src/Core/util/Macros.h \
    libs/Eigen/src/Core/util/Memory.h \
    libs/Eigen/src/Core/util/Meta.h \
    libs/Eigen/src/Core/util/MKL_support.h \
    libs/Eigen/src/Core/util/NonMPL2.h \
    libs/Eigen/src/Core/util/ReenableStupidWarnings.h \
    libs/Eigen/src/Core/util/StaticAssert.h \
    libs/Eigen/src/Core/util/XprHelper.h \
    libs/Eigen/src/Core/Array.h \
    libs/Eigen/src/Core/ArrayBase.h \
    libs/Eigen/src/Core/ArrayWrapper.h \
    libs/Eigen/src/Core/Assign.h \
    libs/Eigen/src/Core/Assign_MKL.h \
    libs/Eigen/src/Core/AssignEvaluator.h \
    libs/Eigen/src/Core/BandMatrix.h \
    libs/Eigen/src/Core/Block.h \
    libs/Eigen/src/Core/BooleanRedux.h \
    libs/Eigen/src/Core/CommaInitializer.h \
    libs/Eigen/src/Core/ConditionEstimator.h \
    libs/Eigen/src/Core/CoreEvaluators.h \
    libs/Eigen/src/Core/CoreIterators.h \
    libs/Eigen/src/Core/CwiseBinaryOp.h \
    libs/Eigen/src/Core/CwiseNullaryOp.h \
    libs/Eigen/src/Core/CwiseTernaryOp.h \
    libs/Eigen/src/Core/CwiseUnaryOp.h \
    libs/Eigen/src/Core/CwiseUnaryView.h \
    libs/Eigen/src/Core/DenseBase.h \
    libs/Eigen/src/Core/DenseCoeffsBase.h \
    libs/Eigen/src/Core/DenseStorage.h \
    libs/Eigen/src/Core/Diagonal.h \
    libs/Eigen/src/Core/DiagonalMatrix.h \
    libs/Eigen/src/Core/DiagonalProduct.h \
    libs/Eigen/src/Core/Dot.h \
    libs/Eigen/src/Core/EigenBase.h \
    libs/Eigen/src/Core/ForceAlignedAccess.h \
    libs/Eigen/src/Core/Fuzzy.h \
    libs/Eigen/src/Core/GeneralProduct.h \
    libs/Eigen/src/Core/GenericPacketMath.h \
    libs/Eigen/src/Core/GlobalFunctions.h \
    libs/Eigen/src/Core/Inverse.h \
    libs/Eigen/src/Core/IO.h \
    libs/Eigen/src/Core/Map.h \
    libs/Eigen/src/Core/MapBase.h \
    libs/Eigen/src/Core/MathFunctions.h \
    libs/Eigen/src/Core/MathFunctionsImpl.h \
    libs/Eigen/src/Core/Matrix.h \
    libs/Eigen/src/Core/MatrixBase.h \
    libs/Eigen/src/Core/NestByValue.h \
    libs/Eigen/src/Core/NoAlias.h \
    libs/Eigen/src/Core/NumTraits.h \
    libs/Eigen/src/Core/PermutationMatrix.h \
    libs/Eigen/src/Core/PlainObjectBase.h \
    libs/Eigen/src/Core/Product.h \
    libs/Eigen/src/Core/ProductEvaluators.h \
    libs/Eigen/src/Core/Random.h \
    libs/Eigen/src/Core/Redux.h \
    libs/Eigen/src/Core/Ref.h \
    libs/Eigen/src/Core/Replicate.h \
    libs/Eigen/src/Core/ReturnByValue.h \
    libs/Eigen/src/Core/Reverse.h \
    libs/Eigen/src/Core/Select.h \
    libs/Eigen/src/Core/SelfAdjointView.h \
    libs/Eigen/src/Core/SelfCwiseBinaryOp.h \
    libs/Eigen/src/Core/Solve.h \
    libs/Eigen/src/Core/SolverBase.h \
    libs/Eigen/src/Core/SolveTriangular.h \
    libs/Eigen/src/Core/StableNorm.h \
    libs/Eigen/src/Core/Stride.h \
    libs/Eigen/src/Core/Swap.h \
    libs/Eigen/src/Core/Transpose.h \
    libs/Eigen/src/Core/Transpositions.h \
    libs/Eigen/src/Core/TriangularMatrix.h \
    libs/Eigen/src/Core/VectorBlock.h \
    libs/Eigen/src/Core/VectorwiseOp.h \
    libs/Eigen/src/Core/Visitor.h \
    libs/Eigen/Dense \
    libs/unsupported/Eigen/OpenGLSupport \
    ui_mainwindow.h

FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag

INCLUDEPATH += src libs glm libs/glew-1.10.0/include
DEPENDPATH += src libs glm libs/glew-1.10.0/include

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
