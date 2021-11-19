#pragma once

#include "../exvis.h"

namespace exvis {
    class MemoryBlock {
    public:
        void render();
        MemoryBlock() = default;

        MemoryBlock(memory_block_2d &memory, float pxs);
    private:
        uint32_t width;
        uint32_t height;

        const char* title;
        float px_size;
    };
}


