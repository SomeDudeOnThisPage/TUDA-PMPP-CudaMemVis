#pragma once

#include "../exvis.h"
#include "GUIElement.h"
#include "../platform/GLFrameBuffer.h"
#include "../platform/GLShader.h"
#include <sstream>
#include <iomanip>

namespace exvis {
    class MemoryBlock : public GUIElement {
    public:
        void render() override;

        bool apply(trace_instruction instruction);
        bool revert(trace_instruction instruction);
        MemoryBlock() = default;

        MemoryBlock(memory_block_info &memory, std::vector<int> &bs, std::vector<int> &mbs, float pxs);
    private:
        std::vector<int> m_max_block_index; // max. block index in grid
        std::vector<int> m_block_size; // max. block index in grid

        uint32_t m_width;  // grid width
        uint32_t m_height; // grid height
        memory_block_info m_memory_info;

        uint32_t m_max_accesses = 1;

        uint32_t m_ld_accesses = 0;
        uint32_t m_st_accesses = 0;

        bool m_render_ld;
        bool m_render_st;

        std::string m_title;
        float m_px_size{};

        GLFrameBuffer* m_buffer;
        GLTexture* m_data_texture; // r = data, b = loads, c = stores
        GLShader* m_shader;

        void render_heatmap();
        void render_texture();
        void set_data_texture(uint32_t column, uint32_t row, float data);
    };
}


