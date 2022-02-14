#include "TraceFile.h"

bool exvis::TraceFile::load(const char *path) {
    std::fstream file(path, std::ios_base::in);

    std::cout << "loading trace file " << path << std::endl;
    std::string line;

    if (file.is_open()) {
        this->m_max_block_index[0] = 0;
        this->m_max_block_index[1] = 0;
        this->m_max_block_index[2] = 0;

        // read meta
        file >> line;
        if (line != "HEADER") {
            return false;
        }
        char ihatecpp;
        file >> line;
        std::stringstream line0_stream(line);
        line0_stream >> this->m_grid_size[0];
        line0_stream.get(ihatecpp);
        line0_stream >> this->m_grid_size[1];
        line0_stream.get(ihatecpp);
        line0_stream >> this->m_grid_size[2];

        file >> line;
        std::stringstream line1_stream(line);
        line1_stream >> this->m_block_size[0];
        line1_stream.get(ihatecpp);
        line1_stream >> this->m_block_size[1];
        line1_stream.get(ihatecpp);
        line1_stream >> this->m_block_size[2];

        std::cout << "Block Size -> [" << this->m_block_size[0] << ", " << this->m_block_size[1] << ", " << this->m_block_size[2] << "]" << std::endl;

        file >> line;
        while (line != "BODY") {
            // create memory block info
            std::stringstream line_stream(line);
            uint32_t width, height, pitch;
            std::string address;
            line_stream >> width;
            line_stream.get(ihatecpp);
            line_stream >> height;
            line_stream.get(ihatecpp);
            line_stream >> pitch;
            line_stream.get(ihatecpp);
            line_stream >> address;
            std::cout << width << " " << height << " " << pitch << " " << address << std::endl;
            file >> line;

            auto* info = new exvis::memory_block_info();
            info->start = (uint64_t) std::strtoll(address.c_str(), (char**) nullptr, 16);
            info->size = width * height * 4;
            info->pitch = pitch;
            info->width = width;
            info->height = height;
            info->data = (float*) calloc(info->width * info->pitch / 4, sizeof(float));
            std::vector<int> st_accesses(info->height * info->width, 0);
            std::vector<int> ld_accesses(info->height * info->width, 0);
            info->st_accesses = st_accesses;
            info->ld_accesses = ld_accesses;
            this->m_blocks.push_back(*info);
        }

        // read instructions
        int lines = 0;
        while (!file.eof()) {
            file >> line;
            lines++;

            size_t pos = line.find(',');
            std::string instruction_string = line.substr(0, pos);

            auto *trace = new exvis::trace_instruction();
            if (instruction_string.find("LDG") != std::string::npos) {
                trace->instruction = exvis::instruction_type::LOAD;
            } else {
                trace->instruction = exvis::instruction_type::STORE;
            }
            line.erase(0, pos + 1);

            // read in block info
            int block_x = std::stoi(line.substr(0, 1));
            line.erase(0, 2);

            int block_y = std::stoi(line.substr(0, 1));
            line.erase(0, 2);

            int block_z = std::stoi(line.substr(0, 1));
            line.erase(0, 2);

            if (block_x > this->m_max_block_index[0]) {
                this->m_max_block_index[0] = block_x;
            }

            if (block_y > this->m_max_block_index[1]) {
                this->m_max_block_index[1] = block_y;
            }

            if (block_z > this->m_max_block_index[2]) {
                this->m_max_block_index[2] = block_z;
            }

            //line.erase(0, 6);

            while ((pos = line.find(',')) != std::string::npos) {
                auto kv = line.substr(0, pos);
                auto address_string = kv.substr(0, 18);
                auto data_string = kv.substr(19, 37);
                auto address = (uint64_t) std::strtoll(address_string.c_str(), (char**) nullptr, 16);
                auto data = (float) std::strtoll(data_string.c_str(), (char**) nullptr, 16);

                trace->address = address;
                trace->data = data;
                this->m_instructions.push_back(*trace);
                line.erase(0, pos + 1);
            }
        }


        // std::cout << "yoo" << std::endl;
        // TODO: dirty hack until trace file format fix
        // this->m_instructions.erase(this->m_instructions.end() - 1);
        std::cout << "TOTAL INSTRUCTIONS: " << this->m_instructions.size() << " " << lines << std::endl;
        std::cout << "MAX BLOCK INDEX: " << this->m_max_block_index[0] << " " << this->m_max_block_index[1] << " " << this->m_max_block_index[2] << std::endl;

    }
    return true;
}
