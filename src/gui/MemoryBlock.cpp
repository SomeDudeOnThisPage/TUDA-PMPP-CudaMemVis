#include <imgui_internal.h>
#include "MemoryBlock.h"

template< typename T > std::string int_to_hex( T i ) {
    std::stringstream stream;
    stream << "0x" << std::setfill ('0') << std::setw(sizeof(T) * 2) << std::hex << i;
    return stream.str();
}

uint64_t ii = 0;
exvis::MemoryBlock::MemoryBlock(memory_block_info &memory, std::vector<int> &bs, std::vector<int> &mbs, float pxs) :
    m_px_size{pxs},
    m_memory_info(memory),
    m_block_size(bs),
    m_max_block_index(mbs) {
    this->m_width = memory.width; // width in elements
    this->m_height = memory.height;
    this->m_title = "Memory Region " + std::to_string(ii++);

    this->m_buffer = new GLFrameBuffer(this->m_width, this->m_height);
    this->m_shader = new GLShader(R"(F:\dev\TU Darmstadt\PMPP\ExVis\resources\shaders\amogus.vs)", R"(F:\dev\TU Darmstadt\PMPP\ExVis\resources\shaders\amogus.fs)");
    this->m_data_texture = new GLTexture(this->m_width, this->m_height);
}

void exvis::MemoryBlock::render_texture() {
    this->m_buffer->bind();
    this->m_shader->bind();
    ImVec2 image_resolution{static_cast<float>(this->m_width), static_cast<float>(this->m_height)};

    this->m_data_texture->bind();
    glUniform2f(glGetUniformLocation(this->m_shader->m_id, "u_resolution"), image_resolution.x, image_resolution.y);
    glUniform1i(glGetUniformLocation(this->m_shader->m_id, "u_data"), 0);

    glUniform1i(glGetUniformLocation(this->m_shader->m_id, "u_ld_enabled"), this->m_render_ld);
    glUniform1i(glGetUniformLocation(this->m_shader->m_id, "u_st_enabled"), this->m_render_st);

    glUniform1i(glGetUniformLocation(this->m_shader->m_id, "u_show_grid_blocks"), u_show_grid_blocks);
    glUniform1f(glGetUniformLocation(this->m_shader->m_id, "u_grid_block_opacity"), u_grid_block_opacity);

    glUniform3f(glGetUniformLocation(this->m_shader->m_id, "u_block_size"), this->m_block_size[0], this->m_block_size[1], this->m_block_size[2]);
    glUniform3f(glGetUniformLocation(this->m_shader->m_id, "u_max_block_index"), this->m_max_block_index[0],
                this->m_max_block_index[1], this->m_max_block_index[2]);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->m_data_texture->unbind();

    ImGui::Image((ImTextureID) this->m_buffer->m_renderbuffer->m_id, {this->m_width * this->m_px_size, this->m_height * this->m_px_size}, ImVec2(0, 0), ImVec2(1, 1));

    this->m_shader->unbind();
    this->m_buffer->unbind();

    ImVec2 window_position{ImGui::GetWindowPos().x + 10, ImGui::GetWindowPos().y + 10};
    if (ImGui::IsMouseHoveringRect(window_position, {window_position.x + this->m_width * this->m_px_size, window_position.y + this->m_height * this->m_px_size})) {
        ImVec2 cursor_position_local{ImGui::GetMousePos().x - window_position.x, ImGui::GetMousePos().y - window_position.y};
        ImGui::BeginTooltip();

        uint8_t x = cursor_position_local.x / this->m_px_size;
        uint8_t y = cursor_position_local.y / this->m_px_size;

        uint32_t position = y * this->m_memory_info.width + x; // invert because of in what orientation we render and display the texture
        uint32_t st_accesses = this->m_memory_info.st_accesses[position];
        uint32_t ld_accesses = this->m_memory_info.ld_accesses[position];

        ImGui::Text("Position : [%d, %d]\n", x, y);
        ImGui::Text("Address  : [%s]\n", int_to_hex(this->m_memory_info.start + (x * this->m_memory_info.pitch + y) * 4).c_str());
        ImGui::Text("Accesses : [ST: %d, LD: %d, SUM: %d]\n", st_accesses, ld_accesses, ld_accesses + st_accesses);
        ImGui::EndTooltip();
    }
}

void exvis::MemoryBlock::render() {
    if (ImGui::Begin(this->m_title.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::Checkbox("LDG", &this->m_render_ld); ImGui::SameLine();
        ImGui::Checkbox("STG", &this->m_render_st); ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

        ImVec2 ws = ImGui::GetWindowSize();
        ws.x -= 2 * ImGuiStyle().WindowPadding.x;
        ws.y -= 4 * ImGui::GetTextLineHeightWithSpacing() + 2 * ImGuiStyle().WindowPadding.y;
        if (ImGui::BeginChild("Canvas", ws, true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
             this->render_texture();
        }
        ImGui::EndChild();
    }
    ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth());
    ImGui::SliderFloat("##scale", &this->m_px_size, 1.0f, 50.0f, "%.0fpx/cell", ImGuiSliderFlags_AlwaysClamp);;
    ImGui::End();
}

void exvis::MemoryBlock::set_data_texture(uint32_t column, uint32_t row, float data) {
    uint32_t position = row * this->m_memory_info.width + column;

    this->m_data_texture->bind();
    // float data = this->m_memory_info.ld_accesses[position];
    float u_data[4];
    u_data[0] = (float) this->m_memory_info.st_accesses[position];
    u_data[1] = (float) this->m_memory_info.ld_accesses[position];
    u_data[2] = (float) this->m_memory_info.st_accesses[position];
    u_data[3] = 1.0f;
    glTexSubImage2D(GL_TEXTURE_2D, 0, column, row, 1, 1, GL_RGBA, GL_FLOAT, &u_data); // switched because imgui texture rotation is weird
    this->m_data_texture->unbind();
}

bool exvis::MemoryBlock::apply(trace_instruction instruction) {
    if (instruction.address >= this->m_memory_info.start && instruction.address < this->m_memory_info.start + this->m_memory_info.pitch * this->m_memory_info.height) {
        uint32_t position = (instruction.address - this->m_memory_info.start);
        uint64_t row = std::floor(position / this->m_memory_info.pitch);
        uint64_t column = std::floor((position % this->m_memory_info.pitch) / 4 /* TODO: dynamic data size parameter? */);

        try {
            switch (instruction.instruction) {
                case exvis::instruction_type::LOAD:
                    this->m_memory_info.ld_accesses.at(row * this->m_memory_info.width + column) += 1;
                    this->set_data_texture(column, row, instruction.data);
                    break;
                case exvis::instruction_type::STORE:
                    this->m_memory_info.st_accesses.at(row * this->m_memory_info.width + column) += 1;
                    this->set_data_texture(column, row, instruction.data);
                    break;
            }
        } catch (...) {
            std::cout << "oh no" << std::endl;
        }

        return true;
    }
    return false;
}

bool exvis::MemoryBlock::revert(trace_instruction instruction) {
    if (instruction.address >= this->m_memory_info.start && instruction.address <= this->m_memory_info.start + this->m_memory_info.pitch * this->m_memory_info.height) {
        uint32_t position = (instruction.address - this->m_memory_info.start);
        uint64_t row = std::floor(position / this->m_memory_info.pitch);
        uint64_t column = std::floor((position % this->m_memory_info.pitch) / 4 /* TODO: data size */);

        if (row == 0 && column == 1) {
            std::cout << "SHITTY FUCK" << std::endl;
        }

        uint32_t pos = row * this->m_memory_info.width + column;
        try {
            switch (instruction.instruction) {
                case exvis::instruction_type::LOAD:
                    this->m_memory_info.ld_accesses.at(pos) = std::max(0, this->m_memory_info.ld_accesses.at(pos) - 1);
                    this->set_data_texture(column, row, instruction.data);
                    break;
                case exvis::instruction_type::STORE:
                    this->m_memory_info.st_accesses.at(pos) -= 1; //std::max(0, this->m_memory_info.st_accesses.at(pos) - 1);
                    this->set_data_texture(column, row, instruction.data);
                    break;
            }
        } catch (std::runtime_error e) {
            std::cout << "fuccc" << std::endl;
        }

        return true;
    }
    return false;
}
