#pragma once
#include <string>
#include <glad/glad.h>
#include "XCommon.h"
#include <glm/glm.hpp>

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertexPath, const std::string& InFragmPath);
    ~OGLProgram();
    void Bind();

    void SetUniform(const std::string& InName, float InValue);
    void SetUniform(const std::string& InName, const Color& InValue);
    void SetUniform(const std::string& InName, const glm::mat4& InValue);
    void SetUniform(const std::string& InName, const glm::vec3& InValue);

    GLuint ProgramId;
private:
    //GLuint ProgramId;
};