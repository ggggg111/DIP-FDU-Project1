# DIP-FDU-Project1

## Introduction

This project has been created for the subject Digital Image Processing at Fudan University. It consists in an Image Editor which can load, save, and edit images, similar to the MS Paint software.

## Author

[Gerard Marcos Freixas](https://github.com/nngg11).

## Features

- Load images in *.jpg/.png/.bmp* formats.
  
- Save images in *.jpg/.png/.bmp* formats.

- Editing tools, including a standard brush, a rubber, circle and square brushes, lines, ellipses, and rectangles. The color and size of the tool can be tweaked.
  
- Editing filters, including grayscale, blur, and negative filtering.

## Controls

- **Left-click:** Use the current selected tool. Interact with the Immediate Use GUI.

- **Middle-click:** Move the loaded image in the canvas.

- **Right-click:** Use the rubber tool.

## Dependencies

- [SDL2](https://www.libsdl.org/)

- [SDL_image 2.0](https://www.libsdl.org/projects/SDL_image/)

- [SDL2_gfx](https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)

- [Dear ImGui](https://github.com/ocornut/imgui)
  
- [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs)

*Note: The dependencies are already included in the project, for Windows 10+ x86 targets.*

## Compatibility

At the moment, the project supports Windows 10+ environments and is compiled for x86 platforms. Note that the source code and libraries used are cross-platform friendly, so they can also be compiled for Linux or Apple systems.

## Instructions

To compile the project, follow the steps below:

**1.** Run *ImageEditor.sln* to open the Visual Studio 2022 solution.

**2.** Set the solution platform to *x86*.

**3.** On the solution explorer, right-click *ImageEditor* and select *Properties*.

**4.** On the *Configuration Properties->Debugging* panel, change the *Working Directory* to *.\workdir*.

To run the release, do:

**1.** Extract the release files to any directory.

**2.** Execute *ImageEditor.exe*.

## Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/).
  
  - [Desktop development with C++ workload](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).
  
- [Windows 10](https://www.microsoft.com/en-us/software-download/windows10).

## Releases

The latest releases can be viewed [here](https://github.com/nngg11/DIP-FDU-Project1/releases).

## Respository

The repository is hosted [here](https://github.com/nngg11/DIP-FDU-Project1).