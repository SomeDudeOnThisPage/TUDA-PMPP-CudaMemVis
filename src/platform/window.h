#pragma once

#include "../exvis.h"
#include <string>
#include <stdexcept>
#include <iostream>

namespace exvis {
    class Window {

    public:
        GLFWwindow *m_window;

        Window() = delete;
        Window(uint16_t width, uint16_t height, const std::string& title);
        ~Window();

        bool client_exit();
        void poll();
        void swap_buffers();

    private:
        uint16_t m_width;
        uint16_t m_height;
    };
}
