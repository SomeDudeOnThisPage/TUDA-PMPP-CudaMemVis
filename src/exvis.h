#pragma once

#include <iostream>

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include "platform/window.h"

struct memory_block_descriptor_1d {
    const char* unique_identifier;

    uint64_t start;     // start memory address
    uint64_t size;      // size in bytes
    uint8_t element_size;
};

struct memory_block_descriptor_2d : memory_block_descriptor_1d {
    uint64_t width;     // row size in bytes
};

template <typename T>
struct memory_block_1d {
    memory_block_descriptor_1d descriptor;
    T* data;
};

struct memory_block_2d {
    memory_block_descriptor_2d descriptor;
    void* data;
};