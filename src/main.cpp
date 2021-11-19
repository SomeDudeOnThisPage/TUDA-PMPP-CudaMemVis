#include "exvis.h"
#include "gui/MemoryBlock.h"

template <typename T>
memory_block_2d create_memory_block(const char* id) {
    memory_block_descriptor_2d descriptor{};
    descriptor.unique_identifier = id;
    descriptor.start = 0x1234567899999999;
    descriptor.size = 128 * 128 * sizeof(T);
    descriptor.width = 128;
    descriptor.element_size = sizeof(T);

    memory_block_2d block{};
    block.descriptor = descriptor;

    return block;
}

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

    memory_block_2d mem0 = create_memory_block<float>("M");
    memory_block_2d mem1 = create_memory_block<float>("N");

    exvis::MemoryBlock arr0(mem0, 5);
    exvis::MemoryBlock arr1(mem1, 5);

    while (!window->ClientExit()) {
        glad_glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        window->Poll();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        arr0.render();
        arr1.render();

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
