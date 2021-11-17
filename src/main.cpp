#include <iostream>

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include "platform/window.h"


int main() {
    exvis::Window *window;

    try {
        window = new exvis::Window(800, 600, "Woo Wee");
    }
    catch(std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ImGuiContext *ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    ImGui_ImplGlfw_InitForOpenGL(window->m_window, true);
    ImGui_ImplOpenGL3_Init("#version 140");

    while (!window->ClientExit()) {
        glad_glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        window->Poll();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Hello World")) {
            ImGui::Text("I'm the Globglogabgalab. I love books, and this basement is a true treasure trove.\n"
                        "\n"
                        "I am the Globglogabgalab; The schwabboldabblewabble gabbleflebablabablab.\n"
                        "\n"
                        "I'm full of schwibbleglibblekind; I am the yeast of thoughts and mind.\n"
                        "\n"
                        "Schwabbledabbleglibbeglabbe schwibbleschwabglab; Dibbledabble schwibbleschwabble glibbleglabschwab.\n"
                        "\n"
                        "Schwabbledabbleglibbeglabbe schwibbleschwabdab; Dibbledabble schwibbleschwabble glibbleschwabglab.\n"
                        "\n"
                        "Ooh hahaha mmmh splendid! Simply delicious, ooomm hahaa haha!\n"
                        "\n"
                        "I am the Globglogabgalab; The schwabboldabblewabble gabbleflebablabablab.\n"
                        "\n"
                        "I'm full of schwibbleglibblekind; I am the yeast of thoughts and mind.\n"
                        "\n"
                        "Schwabbledabbleglibbeglabbe schwibbleschwabglab; Dibbledabble schwibbleschwabble glibbleglabschwab.\n"
                        "\n"
                        "Schwabbledabbleglibbeglabbe schwibbleschwabdab; Dibbledabble schwibbleschwabble glibbleschwabglab.\n"
                        "\n"
                        "Aaaah.");
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->SwapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctx);
    delete window;

    return EXIT_SUCCESS;
}
