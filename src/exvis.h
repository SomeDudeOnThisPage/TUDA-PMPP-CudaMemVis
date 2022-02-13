#pragma once

#ifndef IMGUI_DISABLE
    #define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    #include <imgui.h>
    #include <imgui_impl_glfw.h>
    #include <imgui_impl_opengl3.h>
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#include <vector>
#include <chrono>
#include <iostream>

// ya like globals, kid?
extern bool u_show_grid_blocks;
extern float u_grid_block_opacity;

namespace exvis {
    enum instruction_type : uint8_t {
        STORE = 0x0,
        LOAD = 0x1
    };

    enum message_severity {
        INFO, ERROR
    };

    struct message_t {
        std::string message;
        message_severity severity;
    };

    struct trace_instruction {
        uint8_t instruction;
        uint64_t address;
        float data;
    };

    struct memory_block_info {
        std::string id;
        uint64_t start;  // start address
        uint64_t size;   // size in bytes
        uint64_t width;  // width in bytes
        uint64_t height; // height in bytes
        uint64_t pitch;  // pitch in bytes
        float* data;     // TODO: data tracing
        std::vector<int> ld_accesses; // number of accesses for each "data cell"
        std::vector<int> st_accesses; // number of accesses for each "data cell"
        std::vector<int> max_block_size;
    };
}
