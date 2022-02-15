# CUDA Memory Access Visualization Tool
Hier könnte ihre Werbung stehen!

## Required Libraries
- GLFW 3.3.4+ (https://www.glfw.org/)
- ImGUI 1.85+ (bundled)
- GLAD w/ OpenGL <= v300 (bundled)

## Setup & Build
0. ~~Download and unpack (or clone) GLFW somewhere (e.g. `lib/glfw-3.3.4`-folder).~~
1. GLFW is now included as a git submodule: Initialize git submodules (`git submodule update --init`)
2. ~~Build CMake w/ argument `-DGLFW_DIRECTORY` containing the relative path to GLFWs root folder.~~
3. Build CMake
4. IMPORTANT: Move imgui.ini from `resources/setup` into the build directory, and put it next to the ExVis.exe file (otherwise, you must set up the GUI-layout yourself)
5. Run with a tracefile (.csv) as parameter
6. Success!