#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include "ShaderProgram.h"

GLenum ShaderProgram::compileShader(std::string path, GLenum shaderType) {
    // grab the contents of the file and store the source code in a string
    std::ifstream filestream(path);
    std::string shaderSource((std::istreambuf_iterator<char>(filestream)),
        std::istreambuf_iterator<char>());

    // create and compile the shader
    GLuint shaderHandle = glCreateShader(shaderType);
    const char* shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shaderHandle, 1, &shaderSourcePtr, NULL);
    glCompileShader(shaderHandle);

    // check if compilation was successful
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
        std::cerr << "Error while compiling shader\n" << infoLog << std::endl;
    }

    // return the shader handle
    return shaderHandle;
}

ShaderProgram::ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath) : inUse(false) {
	// create and compile shaders
    GLenum vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLenum fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    // create a shader program, attach both shaders and link them together
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for errors while linking the shaders together
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error while linking shaders\n" << infoLog << std::endl;
    }

    // after creating the shader program we don't need the two shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // return the shader program handle
    this->id = shaderProgram;
}

void ShaderProgram::useProgram() {
	this->inUse = true;
	glUseProgram(this->id);
}

void ShaderProgram::unUseProgram() {
	this->inUse = false;
	glUseProgram(0);
}

void ShaderProgram::deleteProgram() {
	if (this->inUse) {
		glUseProgram(0);
		this->inUse = false;
	}
	glDeleteProgram(this->id);
}