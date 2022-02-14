//
// Created by robin on 27.12.2021.
//
#pragma once

#include "../exvis.h"

#include <vector>
#include <sstream>
#include <fstream>

namespace exvis {
    class TraceFile {
    public:
        std::vector<int> m_max_block_index = std::vector<int>(3);
        std::vector<int> m_grid_size = std::vector<int>(3);
        std::vector<int> m_block_size = std::vector<int>(3);
        std::vector<exvis::memory_block_info> m_blocks;
        std::vector<exvis::trace_instruction> m_instructions;
        bool load(const char* path);
    };
}

