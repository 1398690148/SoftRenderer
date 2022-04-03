1、3采样器崩溃

基于DX11架构的软件光栅化渲染器

## 关于项目

在不借助任何图形库的情况下，从零开始构建这样一个3D渲染系统，初衷是为了彻底了解3D渲染过程。

![image-20220403112302919](E:\SoftRenderer\document\StandardScene.png)

**内置**

使用以内第三方库来构建这个渲染器，包括：

- Windows API
- GLM
- stb_image
- asssimp
- oneTBB

## 入门

**先决条件**

我在Windows平台上构建这个项目。请确保您的系统配备了以下软件。

- cmake：至少3.5版本
- visual studio 2017

**安装**

请为**Win32**平台编译项目。

1.克隆仓库

```
https://github.com/1398690148/SoftRenderer.git
```

2.使用cmake构建项目

```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

​	或者使用cmake-gui也是可以的。

目前已将项目使用到的dll放置到output目录，若修改项目生成目录，需将extern/dll/*.dll复制到对应的示例执行目录（如build/Release）。

## 用法

更多详情请查看examples/![示例图](E:\SoftRenderer\document\示例图.png)

## 功能

- 基础渲染
- 纹理加载
- 模型文件加载
- 深度测试
- 平行光、点光源、聚光灯及Lambert和BlinnPhong光照模型
- Alpha Blend
- Alpha Test
- TBB并行加速
- MSAA![MSAA](https://github.com/1398690148/SoftRenderer/blob/master/document/MSAA.png)

## 接触

1398690148@qq.com
