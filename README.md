# DIP-FDU-Project1

## Introduction

This project has been created for the subject Digital Image Processing at Fudan University. It consists in an Image Editor which can load, save, and edit images, similar to the MS Paint software.

## Author

[Gerard Marcos Freixas](https://github.com/nngg11).

## Features

- Load images in *.jpg*/*.png*/*.bmp* formats.

- Save images in *.jpg*/*.png*/*.bmp* formats.

- Editing tools, including a standard brush, a rubber, circle and square brushes, lines, ellipses, and rectangles. The color and size of the tool can be tweaked.

- Filters:
  - Grayscale.
  - Blur.
  - Gaussian Blur.
  - Negative.
  - Laplace Operator.

- Enhancement:
  - Denoise:
    - Median Blur.
  - Brighten:
    - Laplace.
  - Super Resolution.

- HDR Image Loading.

- Anomaly Detection.

- Style Transfer.

## Controls

- **Left-click:** Use the current selected tool. Interact with the Immediate Use GUI.

- **Middle-click:** Move the loaded image in the canvas.

- **Right-click:** Use the rubber tool.

## Dependencies

- [SDL2](https://www.libsdl.org/).

- [SDL_image 2.0](https://www.libsdl.org/projects/SDL_image/).

- [SDL2_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/).

- [Dear ImGui](https://github.com/ocornut/imgui).

- [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs).

- [OpenCV](https://github.com/opencv/opencv).

- [PyTorch C++](https://pytorch.org/cppdocs/).

## External tools

### Real-ERSGAN

- [Paper](https://arxiv.org/abs/2107.10833).

- [Real-ERSGAN (official)](https://github.com/xinntao/Real-ESRGAN)/[Real-ERSGAN-ncnn-vulkan (official)](https://github.com/xinntao/Real-ESRGAN-ncnn-vulkan).

### FastFlow

- [Paper](https://arxiv.org/abs/2111.07677).

- [FastFlow (unofficial)](https://github.com/gathierry/FastFlow).

### URST & AdaIN

- [Paper 1](https://arxiv.org/abs/2103.11784).

- [URST (official)](https://github.com/czczup/URST).

- [Paper 2](https://arxiv.org/pdf/1703.06868.pdf).

- [AdaIn (official)](https://github.com/xunhuang1995/AdaIN-style).

## Compatibility

At the moment, the project supports Windows 10+ environments and is compiled for x64 platforms.

## Instructions

Before starting, it is needed to add the OpenCV library manually:

**1.** Download [OpenCV 4.5.5](https://github.com/opencv/opencv/releases/tag/4.5.5) library and install it.

**2.** Place the downloaded files along with the *build* and *sources* files inside *ImageEditor/ImageEditor/vendor/OpenCV/*.

**3.** Place both *opencv_world455.dll* and *opencv_world455d.dll* DLLs inside *ImageEditor/ImageEditor/vendor/workdir/*.

**4.** Download [Libtorch 1.11.0 + CUDA 11.3 (Debug)](https://download.pytorch.org/libtorch/cu113/libtorch-win-shared-with-deps-debug-1.11.0%2Bcu113.zip) and [Libtorch 1.11.0 + CUDA 11.3 (Release)](https://download.pytorch.org/libtorch/cu113/libtorch-win-shared-with-deps-1.11.0%2Bcu113.zip).

**5.** Place the downloaded files in the *C:* directory so that the paths are as follows: *C:/libtorch-win-shared-with-deps-debug-1.11.0+cu113/libtorch* and *C:/libtorch-win-shared-with-deps-1.11.0+cu113/libtorch*.

**6.** Download [CUDA 11.3](https://developer.nvidia.com/cuda-11.3.0-download-archive?target_os=Windows&target_arch=x86_64&target_version=10&target_type=exe_local) and install it. The install path should be *C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.3*.

To compile the project, follow the steps below:

**1.** Run *ImageEditor.sln* to open the Visual Studio 2019 solution.

**2.** Set the solution platform to *x64*. You can use both debug or release configurations.

**3.** On the solution explorer, right-click *ImageEditor* and select *Properties*.

**4.** On the *Configuration Properties->Debugging* panel, change the *Working Directory* to *.\workdir*.

To run the release, do:

**1.** Extract the release files to any directory.

**2.** Execute *ImageEditor.exe*.

*Note: The dependencies are already included in the project, for Windows 10+ x64 targets, but not for OpenCV as it is quite a heavy library. Please follow the instructions above.*

## Requirements

- [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/).

  - [Desktop development with C++ workload](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).

- [Windows 10](https://www.microsoft.com/en-us/software-download/windows10).

## Releases

The latest releases can be downloaded [here](https://github.com/nngg11/DIP-FDU-Project1/releases).

## Respository

The repository is hosted [here](https://github.com/nngg11/DIP-FDU-Project1).