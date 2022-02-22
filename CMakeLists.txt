CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(SoftRenderer)

add_compile_definitions(COREAPI_EXPORTS = 1) 
SET(CMAKE_CXX_STANDARD 17)

IF(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
ENDIF()

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Casli/Core CORE_SRCS)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin/)
ADD_LIBRARY(Core SHARED ${CORE_SRCS} ${PROJECT_SOURCE_DIR}/src/Casli/Core/Device.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/CoreAPI.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/DeviceContext.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/IBuffer.h
${PROJECT_SOURCE_DIR}/src/Casli/Core/IPixelShader.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/IVertexShader.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/Resource.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/SamplerState.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/Texture2D.h ./src/Casli/Core/tgaimage.h ./src/Casli/Core/ShaderInput.h
${PROJECT_SOURCE_DIR}/src/Casli/Core/RenderTargetView.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/InputLayout.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/DepthStencilView.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/Core.h)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/bin_d)

SET(RENDERER ./src/Casli/Renderer/VertexBuffer.h ./src/Casli/Renderer/VertexBuffer.cpp ./src/Casli/Renderer/IndexBuffer.h ./src/Casli/Renderer/IndexBuffer.cpp
./src/Casli/Renderer/Texture.h ./src/Casli/Renderer/Texture.cpp ./src/Casli/Scene/Graphics.h ./src/Casli/Scene/Graphics.cpp 
./src/Casli/Renderer/LambertVertexShader.h ./src/Casli/Renderer/LambertPixelShader.h ./src/Casli/Renderer/Sampler.h ./src/Casli/Renderer/Sampler.cpp ./src/Casli/Renderer/Bindable.h ./src/Casli/Renderer/Bindable.cpp
./src/Casli/Renderer/ConstantBuffer.h ./src/Casli/Renderer/ConstantBuffer.cpp)

SET(TEST ./src/Casli/Test/Box.h ./src/Casli/Test/Box.cpp ./src/Casli/Test/Plane.h ./src/Casli/Test/Plane.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${RENDERER} ${TEST} ./src/main.cpp ./src/Casli/Scene/App.h ./src/Casli/Scene/App.cpp ./src/Platform/Window.h ./src/Platform/Window.cpp ./src/Platform/CasliWin.h  
./src/Casli/Scene/Camera.h ./src/Casli/Scene/Camera.cpp ./src/Casli/Scene/Model.h ./src/Casli/Scene/Model.cpp ./src/Casli/Scene/Mesh.h ./src/Casli/Scene/Mesh.cpp)
#设置release模式下关闭cmd窗口
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thirdparty)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Core)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ../output/bin_d/assimp-vc141-mtd)