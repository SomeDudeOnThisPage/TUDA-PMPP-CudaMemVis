//
// Created by robin on 17.01.2022.
//

#include "GLTexture.h"

GLTexture::GLTexture(uint32_t width, uint32_t height) : GLAsset(), m_width{width}, m_height{height} {
    glGenTextures(1, &this->m_id);
    std::cout << width << " " << height << std::endl;
    glBindTexture(GL_TEXTURE_2D, this->m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->m_width, this->m_height, 0, GL_RGBA, GL_FLOAT, 0);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &this->m_id);
}

void GLTexture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->m_id);
}

void GLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}