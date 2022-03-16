CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(SoftRenderer)

add_compile_definitions(COREAPI_EXPORTS = 1) 
add_compile_definitions(RENDERERAPI_EXPORTS = 1) 
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

IF(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
ENDIF()

set(BINDABLE 
src/Casli/Renderer/Bindable/Bindable.h
src/Casli/Renderer/Bindable/Bindable.cpp
src/Casli/Renderer/Bindable/Blender.h
src/Casli/Renderer/Bindable/Blender.cpp
src/Casli/Renderer/Bindable/IndexBuffer.h
src/Casli/Renderer/Bindable/IndexBuffer.cpp
src/Casli/Renderer/Bindable/PixelConstantBuffer.h 
src/Casli/Renderer/Bindable/PixelConstantBuffer.cpp
src/Casli/Renderer/Bindable/Sampler.h
src/Casli/Renderer/Bindable/Sampler.cpp
src/Casli/Renderer/Bindable/Texture.h
src/Casli/Renderer/Bindable/Texture.cpp
src/Casli/Renderer/Bindable/VertexBuffer.h
src/Casli/Renderer/Bindable/VertexBuffer.cpp
src/Casli/Renderer/Bindable/VertexConstantBuffer.h
src/Casli/Renderer/Bindable/VertexConstantBuffer.cpp
)

set(LIGHT 
src/Casli/Renderer/Light/Light.h
src/Casli/Renderer/Light/Light.cpp
src/Casli/Renderer/Light/DirectionalLight.h
src/Casli/Renderer/Light/DirectionalLight.cpp
src/Casli/Renderer/Light/PointLight.h
src/Casli/Renderer/Light/PointLight.cpp
src/Casli/Renderer/Light/SpotLight.h
src/Casli/Renderer/Light/SpotLight.cpp
)

set(RENDERER
src/Casli/Renderer/RendererAPI.h
src/Casli/Renderer/Drawable.h
src/Casli/Renderer/Drawable.cpp
src/Casli/Renderer/Camera.h
src/Casli/Renderer/Camera.cpp
src/Casli/Renderer/Mesh.h
src/Casli/Renderer/Mesh.cpp
src/Casli/Renderer/Model.h
src/Casli/Renderer/Model.cpp
src/Casli/Renderer/Graphics.h 
src/Casli/Renderer/Graphics.cpp
src/Casli/Renderer/SceneParse.h
src/Casli/Renderer/SceneParse.cpp
)

set(SHADER 
src/Casli/Shader/LambertPixelShader.h
src/Casli/Shader/LambertVertexShader.h
src/Casli/Shader/BlinnPhongPS.h
src/Casli/Shader/SampleTextureVS.h
src/Casli/Shader/SampleTexturePS.h
src/Casli/Shader/AlphaTestVS.h
src/Casli/Shader/AlphaTestPS.h
src/Casli/Shader/AlphaBlendVS.h
src/Casli/Shader/AlphaBlendPS.h
src/Casli/Shader/PointLightPS.h
)

set(TEST 
src/Casli/Test/Box.h 
src/Casli/Test/Box.cpp 
src/Casli/Test/Plane.h 
src/Casli/Test/Plane.cpp
src/Casli/Test/AlphaPlane.h 
src/Casli/Test/AlphaPlane.cpp
)

set(PLATFORM
src/Platform/Window.h
src/Platform/Window.cpp
src/Platform/Keyboard.h
src/Platform/Keyboard.cpp
src/Platform/CasliTimer.h
src/Platform/CasliTimer.cpp
src/Platform/Mouse.h
src/Platform/Mouse.cpp
src/Platform/CasliMath.h
src/Platform/CasliWin.h
)

source_group( Bindable FILES ${BINDABLE})
source_group( Light FILES ${LIGHT})
source_group( Renderer FILES ${RENDERER})
source_group( Test FILES ${TEST})
source_group( Scene FILES ${SCENE})
source_group( Platform FILES ${PLATFORM})
source_group( Shader FILES ${SHADER})

#设置库生成目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/output/bin_d/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/output/bin/)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Casli/Core CORE_SRCS)
ADD_LIBRARY(Core SHARED ${CORE_SRCS} ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRBlendState.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRCoreAPI.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRDeviceContext.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/SRIBuffer.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRDevice.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRIVertexShader.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRIPixelShader.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/Utils.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRSamplerState.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRTexture2D.h ./src/Casli/Core/tgaimage.h 
./src/Casli/Core/SRShaderInput.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRRenderTargetView.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRInputLayout.h ${PROJECT_SOURCE_DIR}/src/Casli/Core/SRDepthStencilView.h 
${PROJECT_SOURCE_DIR}/src/Casli/Core/SRCore.h)

if (CMAKE_BUILD_TYPE EQUAL Debug)
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/bin_d)
else ()
	LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/bin)
endif()

ADD_LIBRARY(Renderer SHARED ${PLATFORM} ${RENDERER} ${BINDABLE} ${LIGHT} ${TEST} ${SHADER})

target_link_libraries(Core ../lib/TBBlib/ia32/tbb12 ../lib/TBBlib/ia32/tbb12_debug)
TARGET_LINK_LIBRARIES(Renderer Core)
TARGET_LINK_LIBRARIES(Renderer ../lib/assimp-vc141-mtd)

#MipMap测试场景
add_executable(MipMap src/Casli/Scene/MipMap/App.h src/Casli/Scene/MipMap/App.cpp src/Casli/Scene/MipMap/main.cpp)
TARGET_LINK_LIBRARIES(MipMap Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(MipMap PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#点光源测试场景
add_executable(PointLight src/Casli/Scene/PointLight/App.h src/Casli/Scene/PointLight/App.cpp src/Casli/Scene/PointLight/main.cpp)
TARGET_LINK_LIBRARIES(PointLight Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(PointLight PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# Alpha Test
add_executable(AlphaTest src/Casli/Scene/AlphaTest/App.h src/Casli/Scene/AlphaTest/App.cpp src/Casli/Scene/AlphaTest/main.cpp)
TARGET_LINK_LIBRARIES(AlphaTest Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(AlphaTest PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# Alpha Blend
add_executable(AlphaBlend src/Casli/Scene/AlphaBlend/App.h src/Casli/Scene/AlphaBlend/App.cpp src/Casli/Scene/AlphaBlend/main.cpp)
TARGET_LINK_LIBRARIES(AlphaBlend Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(AlphaBlend PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# Bilinear
add_executable(Bilinear src/Casli/Scene/Bilinear/App.h src/Casli/Scene/Bilinear/App.cpp src/Casli/Scene/Bilinear/main.cpp)
TARGET_LINK_LIBRARIES(Bilinear Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(Bilinear PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#点光源测试场景
add_executable(BlinnPhong src/Casli/Scene/BlinnPhong/App.h src/Casli/Scene/BlinnPhong/App.cpp src/Casli/Scene/BlinnPhong/main.cpp)
TARGET_LINK_LIBRARIES(BlinnPhong Renderer)
#设置release模式下关闭cmd窗口
Set_Target_Properties(BlinnPhong PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/Casli/Renderer)
include_directories(${PROJECT_SOURCE_DIR}/src/Casli/Renderer/Bindable)
include_directories(${PROJECT_SOURCE_DIR}/src/Casli/Renderer/Light)
include_directories(${PROJECT_SOURCE_DIR}/src/Casli/Shader)
include_directories(${PROJECT_SOURCE_DIR}/src/Casli/Test)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/oneapi-tbb-2021.5.0/include)