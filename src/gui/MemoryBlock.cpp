#include "MemoryBlock.h"

exvis::MemoryBlock::MemoryBlock(memory_block_2d &memory, float pxs) : px_size{pxs} {
    this->width = memory.descriptor.width; // width in elements
    this->height = memory.descriptor.size / memory.descriptor.element_size / this->width;

    this->title = memory.descriptor.unique_identifier;
}

void exvis::MemoryBlock::render() {
    if (ImGui::Begin(this->title)) {
        if (ImGui::Button("Scale Up")) {
            this->px_size++;
        }

        if (ImGui::Button("Scale Down")) {
            this->px_size--;
        }

        ImGui::SetNextWindowSizeConstraints({0, 0}, {
                (float) this->width * this->px_size + ImGui::GetStyle().WindowPadding.x * 4 + ImGui::GetStyle().WindowBorderSize,
                (float) this->height * this->px_size + ImGui::GetStyle().WindowPadding.y * 4 + ImGui::GetStyle().WindowBorderSize
        });
        if (ImGui::BeginChild("Canvas", {0, 0}, 1,ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            auto base = ImGui::GetCursorScreenPos();
            auto dl = ImGui::GetWindowDrawList();

            float min_y = std::max(0.0f, ImGui::GetScrollX() / this->px_size - 1);
            float min_x = std::max(0.0f, ImGui::GetScrollY() / this->px_size - 1);

            ImVec2 child_size = ImGui::GetWindowSize();
            float max_x = min_x + child_size.y / this->px_size;
            float max_y = min_y + child_size.x / this->px_size;

            for (unsigned int i = min_x; i < fmin(max_x, 128.0f); i++) {
                for (unsigned int j = min_y; j < fmin(max_y, 128.0f); j++) {
                    ImVec2 p0 = {base.x + j * this->px_size, base.y + i * this->px_size};
                    ImVec2 p1 = {base.x + (j + 1) * this->px_size - 1.0f, base.y + (i + 1) * this->px_size - 1.0f};

                    dl->AddRectFilled(p0, p1, ImGui::ColorConvertFloat4ToU32(ImVec4{1.0f, 0.4f, 0.4f, 0.4f}));

                    if (ImGui::IsMouseHoveringRect(p0, {p1.x + 1.0f, p1.y + 1.0f})) {
                        ImGui::Text("[%3d, %3d]\n", i, j);
                        ImGui::BeginTooltip();
                        ImGui::Text("[%3d, %3d]\n", i, j);
                        ImGui::EndTooltip();
                    }
                }
            }
            ImGui::SetCursorPos(ImVec2{this->width * this->px_size + ImGui::GetStyle().WindowPadding.x * 2,
                                       this->height * this->px_size + ImGui::GetStyle().WindowPadding.y * 2});
        }
        ImGui::EndChild();
    }
    ImGui::End();
}