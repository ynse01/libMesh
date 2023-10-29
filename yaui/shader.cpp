#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ostream>

void ErrorCallback(int error, const char *description) {
	std::cerr << "Error: " << description << std::endl;
}

YetAnotherUI::Shader::Shader(const std::string &vertexCode, const std::string &fragmentCode)
: mVertexShaderCode(vertexCode), mFragmentShaderCode(fragmentCode)
{
}

void YetAnotherUI::Shader::Initialize()
{
    if (mShaderId > 0) return;
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSource = mVertexShaderCode.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = mFragmentShaderCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    mShaderId = glCreateProgram();
    glAttachShader(mShaderId, vertexShader);
    glAttachShader(mShaderId, fragmentShader);
    glLinkProgram(mShaderId);
    // check for linking errors
    glGetProgramiv(mShaderId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void YetAnotherUI::Shader::Render()
{
    glUseProgram(mShaderId);
}

void YetAnotherUI::Shader::Destroy()
{
    glDeleteShader(mShaderId);
    mShaderId = 0;
}

void YetAnotherUI::Shader::setUniform(unsigned int index, libMesh::Color color)
{
    glUniform4f(index, color.red, color.green, color.blue, color.alpha);
}

void YetAnotherUI::Shader::setUniform(unsigned int index, libMesh::Point3 point)
{
    glUniform3f(index, point.x, point.y, point.z);
}

void YetAnotherUI::Shader::setUniform(unsigned int index, libMesh::Point2 point)
{
    glUniform2f(index, point.x, point.y);
}
