//
// Created by robin on 17.01.2022.
//
#pragma once

#include "GLAsset.h"
#include "GLBindable.h"

class GLTexture : public GLAsset, GLBindable {
public:
    uint32_t m_width, m_height;

    GLTexture(uint32_t width, uint32_t height);
    ~GLTexture();

    void bind() override;
    void unbind() override;
};
