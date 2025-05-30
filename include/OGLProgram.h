#pragma once
#include <string>
#include <glad/glad.h>
#include "XCommon.h"

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertexPath, const std::string& InFragmPath);
    ~OGLProgram();
    void Bind();

    void SetUniform(const std::string& InName, float InValue);
    void SetUniform(const std::string& InName, const Color& InValue);

    GLuint ProgramId;
private:
    //GLuint ProgramId;
};