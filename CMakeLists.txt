CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(SoftRenderer)

add_compile_definitions(COREAPI_EXPORTS = 1) 
SET(CMAKE_CXX_STANDARD 17)

IF(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
ENDIF()

set(RENDERER 
src/Casli/Renderer/Bindable.h
src/Casli/Renderer/Bindable.cpp
src/Casli/Renderer/Blender.h
src/Casli/Renderer/Blender.cpp
src/Casli/Renderer/Drawable.h
src/Casli/Renderer/Drawable.cpp
src/Casli/Renderer/IndexBuffer.h
src/Casli/Renderer/IndexBuffer.cpp
src/Casli/Renderer/Light.h
src/Casli/Renderer/DirectionalLight.h
src/Casli/Renderer/DirectionalLight.cpp
src/Casli/Renderer/PointLight.h
src/Casli/Renderer/PointLight.cpp
src/Casli/Renderer/SpotLight.h
src/Casli/Renderer/SpotLight.cpp
src/Casli/Renderer/PixelConstantBuffer.h 
src/Casli/Renderer/PixelConstantBuffer.cpp
src/Casli/Renderer/Sampler.h
src/Casli/Renderer/Sampler.cpp
src/Casli/Renderer/Texture.h
src/Casli/Renderer/Texture.cpp
src/Casli/Renderer/VertexBuffer.h
src/Casli/Renderer/VertexBuffer.cpp
src/Casli/Renderer/VertexConstantBuffer.h
src/Casli/Renderer/VertexConstantBuffer.cpp
)

set(SCENE
src/Casli/Scene/App.h
src/Casli/Scene/App.cpp
src/Casli/Scene/Camera.h
src/Casli/Scene/Camera.cpp
src/Casli/Scene/Graphics.h
src/Casli/Scene/Graphics.cpp
src/Casli/Scene/Mesh.h
src/Casli/Scene/Mesh.cpp
src/Casli/Scene/Model.h
src/Casli/Scene/Model.cpp
src/Casli/Scene/LambertPixelShader.h
src/Casli/Scene/LambertVertexShader.h
)

set(TEST 
src/Casli/Test/Box.h 
src/Casli/Test/Box.cpp 
src/Casli/Test/Plane.h 
src/Casli/Test/Plane.cpp
)

set(PLATFORM
src/Platform/Window.h
src/Platform/Window.cpp
src/Platform/CasliWin.h
)

source_group( Renderer FILES ${RENDERER})
source_group( Test FILES ${TEST})
source_group( Scene FILES ${SCENE})
source_group( Platform FILES ${PLATFORM})

#设置库生成目录
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin_d/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin/)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Casli/Core CORE_SRCS)
ADD_LIBRARY(Core SHARED ${CORE_SRCS} ${PROJECT_SOURCE_DIR}/src/Casli/Core/BlendState.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/CoreAPI.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/DeviceContext.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/IBuffer.h
${PROJECT_SOURCE_DIR}/src/Casli/Core/Device.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/IVertexShader.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/Utils.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SamplerState.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/Texture2D.h ./src/Casli/Core/tgaimage.h ./src/Casli/Core/ShaderInput.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/RenderTargetView.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/InputLayout.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/DepthStencilView.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/Core.h)

if (CMAKE_BUILD_TYPE EQUAL Debug)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/bin_d)
else ()
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/bin)
endif()

ADD_EXECUTABLE(${PROJECT_NAME} ${PLATFORM} ${RENDERER} ${SCENE} ${TEST} ./src/main.cpp)
#设置release模式下关闭cmd窗口
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thirdparty)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Core)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ../output/bin_d/assimp-vc141-mtd)