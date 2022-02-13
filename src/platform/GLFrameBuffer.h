//
// Created by robin on 24.01.2022.
//
#pragma once

#include "GLAsset.h"
#include "GLTexture.h"

class GLFrameBuffer : GLAsset, GLBindable {
public:

    GLTexture* m_renderbuffer;
    uint32_t m_width, m_height;

    GLFrameBuffer(uint32_t width, uint32_t height);
    ~GLFrameBuffer();

    void bind() override;
    void unbind() override;

    void prepare_render() {
        this->bind();
        glViewport(0, 0, this->m_width, this->m_height);
    }
};
