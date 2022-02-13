//
// Created by robin on 24.01.2022.
//
#pragma once

#include "GLAsset.h"
#include "GLBindable.h"

class GLShader : public GLAsset, GLBindable {
public:
    GLShader(const std::string& vs, const std::string& fs);
    ~GLShader();

    void bind() override;
    void unbind() override;
};
