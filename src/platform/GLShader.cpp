//
// Created by robin on 24.01.2022.
//

#include <fstream>
#include "GLShader.h"

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0)
    {
        std::vector<GLchar> infoLog(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog.data());
        std::cout << infoLog.data() << std::endl;
    }
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    if (infologLength > 0)
    {
        std::vector<GLchar> infoLog(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog.data());
        std::cout << infoLog.data() << std::endl;
    }
}

GLuint readShaders(const char *vertexShaderFilename, const char *fragmentShaderFilename) {
    // read vertex shader if possible
    std::ifstream vin(vertexShaderFilename, std::ifstream::in | std::ios::binary);
    std::noskipws(vin);
    if (!vin) {
        std::cout << "readShaders(): " << vertexShaderFilename << " not found!" << std::endl;
        return 0;
    }
    std::string vsource(std::istream_iterator<char>(vin), {});
    vin.close();

    // read fragment shader if possible
    std::ifstream fin(fragmentShaderFilename, std::ifstream::in | std::ios::binary);
    std::noskipws(fin);
    if (!fin) {
        std::cout << "readShaders(): " << fragmentShaderFilename << " not found!" << std::endl;
        return 0;
    }
    std::string fsource(std::istream_iterator<char>(fin), {});
    fin.close();

    const char *vShaderSourcePtr = vsource.c_str();
    const char *fShaderSourcePtr = fsource.c_str();

    // create shaders, set source and compile
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSourcePtr, nullptr);
    glShaderSource(fragmentShader, 1, &fShaderSourcePtr, nullptr);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // create a program, attach both shaders and link the program (shaders can be deleted now)
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // check of all was successful and return the programID
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        std::cout << "===== Vertex Shader =====" << std::endl << vsource << std::endl;
        std::cout << std::endl;
        std::cout << "===== Vertex Shader Info Log =====" << std::endl;
        printShaderInfoLog(vertexShader);
        std::cout << std::endl;
        std::cout << "===== Fragment Shader =====" << std::endl << fsource << std::endl;
        std::cout << std::endl;
        std::cout << "===== Fragment Shader Info Log =====" << std::endl;
        printShaderInfoLog(fragmentShader);
        std::cout << std::endl;
        std::cout << "===== Program Info Log =====" << std::endl;
        printProgramInfoLog(program);
        std::cout << std::endl;
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}

GLShader::GLShader(const std::string& vs, const std::string& fs) {
    this->m_id = readShaders(vs.c_str(), fs.c_str());
}

GLShader::~GLShader() {
    glDeleteProgram(this->m_id);
}

void GLShader::bind() {
    glUseProgram(this->m_id);
}

void GLShader::unbind() {
    glUseProgram(GL_NONE);
}
