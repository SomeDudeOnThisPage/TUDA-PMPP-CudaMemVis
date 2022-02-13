//
// Created by robin on 10.01.2022.
//
#pragma once

#include "exvis.h"
#include "platform/window.h"
#include "platform/TraceFile.h"
#include "gui/MemoryBlock.h"

namespace exvis {
    class Application {
    public:
        Application();
        ~Application();
    private:
        void apply_instructions();
        void imgui_init();
        void imgui_style();

        void load_trace_file(const std::string path);
        void exit();

        exvis::Window *m_window;
        exvis::TraceFile *m_trace_file;
        std::vector<exvis::GUIElement> m_gui;
    };
}


