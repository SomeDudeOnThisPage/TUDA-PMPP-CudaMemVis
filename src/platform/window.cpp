#include "window.h"

namespace exvis {
    Window::Window(const uint16_t width, const uint16_t height, const std::string& title) : m_width{width}, m_height{height} {
        if (!glfwInit()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // We require OpenGL version #2.0 to render ImGui.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_TRUE);

        this->m_window = glfwCreateWindow(this->m_width, this->m_height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(this->m_window);
    }

    Window::~Window() {
        std::cout << "Destroying Window" << std::endl;
        glfwDestroyWindow(this->m_window);
        glfwTerminate();
    }

    bool Window::ClientExit() {
        return glfwWindowShouldClose(this->m_window);
    }

    void Window::Poll() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(this->m_window);
    }
}
