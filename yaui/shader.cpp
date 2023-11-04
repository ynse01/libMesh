#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ostream>

void ErrorCallback(int code, const char *description);
void ErrorCallback(int code, const char *desc0, const char *desc1);

YetAnotherUI::Shader::Shader(const char *vertexCode, const char *fragmentCode)
: mVertexShaderCode(vertexCode), mFragmentShaderCode(fragmentCode)
{
    mShaderId = 0;
}

YetAnotherUI::Shader::~Shader()
{
    if (mShaderId != 0) {
        glDeleteProgram(mShaderId);
    }
    mShaderId = 0;
}

void YetAnotherUI::Shader::Initialize()
{
    if (mShaderId != 0) return;
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &mVertexShaderCode, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        ErrorCallback(success, "SHADER::VERTEX::COMPILATION_FAILED", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &mFragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        ErrorCallback(success, "SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
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
        ErrorCallback(success, "SHADER::PROGRAM::LINKING_FAILED\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //std::cout << "Finished linking shader with id:  " << mShaderId << std::endl;
}

void YetAnotherUI::Shader::Render()
{
    if (mShaderId == 0) {
        ErrorCallback(0, "Trying to render using an uninitialized shader.");
    }
    glUseProgram(mShaderId);
}

void YetAnotherUI::Shader::Destroy()
{
    if (mShaderId != 0) {
        glDeleteShader(mShaderId);
    }
    mShaderId = 0;
}

int YetAnotherUI::Shader::getIndexOfUniform(const char *name)
{
    return glGetUniformLocation(mShaderId, name);
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
