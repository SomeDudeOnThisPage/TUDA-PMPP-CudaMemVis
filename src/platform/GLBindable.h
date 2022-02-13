//
// Created by robin on 24.01.2022.
//
#pragma once

class GLBindable {
public:
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
