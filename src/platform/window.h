#pragma once

#include <GLFW/glfw3.h>
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

        bool ClientExit();
        void Poll();
        void SwapBuffers();

    private:
        uint16_t m_width;
        uint16_t m_height;
    };
}
