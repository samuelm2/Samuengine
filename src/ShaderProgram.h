#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ShaderProgram {
public:
    GLuint id;
    bool inUse;
    ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
    void useProgram();
    void unUseProgram();
    void deleteProgram();
private:
    GLenum compileShader(std::string path, GLenum shaderType);
};