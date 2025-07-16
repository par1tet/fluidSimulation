#pragma once

#include <string>
#include <glad/glad.h>

std::string loadShaderSource(const char* filePath);
GLuint createShader(const char* filePath, GLenum shaderType);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);