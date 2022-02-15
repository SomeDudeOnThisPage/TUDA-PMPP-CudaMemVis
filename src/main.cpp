#include "exvis.h"
#include "platform/window.h"
#include "platform/TraceFile.h"
#include "gui/MemoryBlock.h"

#include <chrono>


// mmm yes good software engineering
bool close = false;

exvis::TraceFile *file;
std::vector<exvis::MemoryBlock> gui_memory_blocks;
std::vector<exvis::message_t> messages;
int current_instruction = 0;
int max_instructions = 0;

int current_instruction_processing_count = 32;

bool u_show_grid_blocks = false;
float u_grid_block_opacity = 0.75f;

void apply_instructions_to(int to) {
    int i = current_instruction;
    int processed = 0;

    auto start = std::chrono::system_clock::now();

    if (to < 0) {
        to = 1;
    } else if (to > file->m_instructions.size()) {
        to = file->m_instructions.size();
    }

    while (i != to) {
        for (auto &info : gui_memory_blocks) {
            if (i <= file->m_instructions.size() && i >= 0) {
                try {
                    if (i < to) {
                        info.apply(file->m_instructions[i]);
                    } else if (i > to) {
                        // std::cout << "to=" << to << " current=" << current_instruction << " i=" << i << std::endl;
                        info.revert(file->m_instructions[i - 1]);
                    }
                } catch (...) {
                }
            }
        }

        if (i < to) {
            i++;
        } else if (i > to) {
            i--;
        }
        processed++;
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;

    exvis::message_t msg{};
    msg.message = "Processed " + std::to_string(processed) + " instructions in " + std::to_string((double) elapsed.count() / 1000000.0) + "ms";
    msg.severity = exvis::message_severity::INFO;
    messages.push_back(msg);
    current_instruction = to;
}

void render_main_ui() {
    if (messages.size() > 100) {
        messages.erase(messages.begin(), messages.begin() + (messages.size() - 100));
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            //if (ImGui::MenuItem("Load Exercise 1 Data File...")) {}
            //if (ImGui::MenuItem("Load Exercise 2 Data File...")) {}
            if (ImGui::MenuItem("Exeunt")) { close = true; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            ImGui::InputInt("Instruction Counter Step", &current_instruction_processing_count, 32, 1024, ImGuiInputTextFlags_CharsDecimal);
            ImGui::Checkbox("Show Grid Blocks", &u_show_grid_blocks);
            ImGui::InputFloat("Gird Block Mix Factor", &u_grid_block_opacity);
            ImGui::EndMenu();
        }
    } ImGui::EndMainMenuBar();

    // How the FUCK does IMGUI Dockspace work?!??!?!?
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::Begin("Shared Memory Blocks")) {
        ImGui::Text("Hello, I am a placeholder window for shared memory segments :D");
        if (ImGui::BeginTabBar("Blocks")) {
            for (int i = 0; i < 32; i++) {
                if (ImGui::BeginTabItem(std::to_string(i).c_str())) {
                    ImGui::Text("Yoo");
                    ImGui::EndTabItem();
                }
            }
        } ImGui::EndTabBar();
    } ImGui::End();

    if (ImGui::Begin("Console")) {
        if (ImGui::BeginChild("##console_messages", {0, 0}, true)) {
            for (const auto& msg : messages) {
                ImGui::Text("%s", msg.message.c_str());
            }
            ImGui::SetScrollHereY();
        } ImGui::EndChild();
    } ImGui::End();

    ImGui::SetNextWindowSize({0, ImGui::GetTextLineHeightWithSpacing()}, ImGuiCond_Always);
    if (ImGui::Begin("Instruction Counter", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {10.0f, 10.0f});
        ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
        int tmp = current_instruction;
        if (ImGui::SliderInt("##instruction_counter", &tmp, 0, max_instructions)) {
            std::cout << "apply to " << tmp << std::endl;
            apply_instructions_to(tmp);
        }
        ImGui::SetCursorPos({ImGui::GetWindowSize().x / 2 - (83 + 3.5f * ImGuiStyle().ItemSpacing.x), ImGui::GetCursorPos().y});
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("<|")) {
            apply_instructions_to(current_instruction - 32);
        } ImGui::SameLine();
        ImGui::PopButtonRepeat();
        if (ImGui::Button("<<")) {
            apply_instructions_to(current_instruction - 32);
        } ImGui::SameLine();
        if (ImGui::Button("<")) {
            apply_instructions_to(current_instruction - 1);
        } ImGui::SameLine();
        if (ImGui::Button(">")) {
            apply_instructions_to(current_instruction + 1);
        } ImGui::SameLine();
        if (ImGui::Button(">>")) {
            apply_instructions_to(current_instruction + 32);
        } ImGui::SameLine();
        ImGui::PushButtonRepeat(true);
        if (ImGui::Button("|>")) {
            apply_instructions_to(current_instruction + 32);
        } ImGui::SameLine();
        ImGui::PopButtonRepeat();
        ImGui::PopStyleVar();
    } ImGui::End();
}

void imgui_style() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../resources/fonts/Roboto-Regular.ttf", 16);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.GrabRounding                           = style.FrameRounding = 2.3f;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Please specify a file path to your tracefile." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string file_path = argv[1];

    u_grid_block_opacity = 0.75f;
    u_show_grid_blocks = false;
    exvis::Window *window;

    file = new exvis::TraceFile();
    file->load(file_path.c_str());

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
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imgui_style();

    ImGui_ImplGlfw_InitForOpenGL(window->m_window, true);
    ImGui_ImplOpenGL3_Init("#version 140");

    for (auto &m_block : file->m_blocks) {
        exvis::MemoryBlock block(m_block, file->m_block_size, file->m_max_block_index, 16);
        gui_memory_blocks.push_back(block);
    }


    max_instructions = file->m_instructions.size();
    current_instruction = 0;

    while (!window->client_exit() && !close) {
        glad_glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        window->poll();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        render_main_ui();

        if (glfwGetKey(window->m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            apply_instructions_to(current_instruction - current_instruction_processing_count);
        }

        if (glfwGetKey(window->m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            apply_instructions_to(current_instruction + current_instruction_processing_count);
        }

        for (auto &gui_memory_block : gui_memory_blocks) {
            gui_memory_block.render();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->swap_buffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(ctx);
    delete window;

    return EXIT_SUCCESS;
}
