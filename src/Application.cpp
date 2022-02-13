//
// Created by robin on 10.01.2022.
//
#include "Application.h"

void exvis::Application::apply_instructions() {

}

exvis::Application::Application() {
    this->m_trace_file = new exvis::TraceFile();
    this->m_trace_file->load(R"(F:\dev\TU Darmstadt\PMPP\ExVis\src\testfile)");

    this->m_window = new exvis::Window(800, 600, "exvis");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize OpenGL context.");
    }

    ImGuiContext *ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imgui_style();

    ImGui_ImplGlfw_InitForOpenGL(this->m_window->m_window, true);
    ImGui_ImplOpenGL3_Init("#version 140");

    for (auto &m_block : this->m_trace_file->m_blocks) {
        auto block = new exvis::MemoryBlock(m_block, 16);
        this->m_gui.push_back(*block);
    }
}
