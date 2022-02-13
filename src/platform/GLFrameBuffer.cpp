//
// Created by robin on 24.01.2022.
//

#include "GLFrameBuffer.h"

GLFrameBuffer::GLFrameBuffer(uint32_t width, uint32_t height) : GLAsset(), m_width{width}, m_height{height} {
    glGenFramebuffers(1, &this->m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
    this->m_renderbuffer = new GLTexture(this->m_width, this->m_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_renderbuffer->m_id, 0);

    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FAILED TO CREATE FRAMEBUFFER" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

GLFrameBuffer::~GLFrameBuffer() {
    delete this->m_renderbuffer;
    glDeleteFramebuffers(1, &this->m_id);
}

void GLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_id);
}

void GLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}
