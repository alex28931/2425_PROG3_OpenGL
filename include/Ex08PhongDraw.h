#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Ex08PhongDraw 
{
public:
    Ex08PhongDraw();
    ~Ex08PhongDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    class OGLProgram* Program;
    class OGLTexture* StormTexture;
    glm::mat4 View;
    glm::mat4 Projection;
    std::vector<float> Vertices;
};