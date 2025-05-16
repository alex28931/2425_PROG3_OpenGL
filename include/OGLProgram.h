#pragma once
#include <string>
#include <glad/glad.h>

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertexPath, const std::string& InFragmPath);
    ~OGLProgram();
    void Bind();
private:
    GLuint ProgramId;
};