#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Ex10LightDraw
{
public:
    Ex10LightDraw();
    ~Ex10LightDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint LightVao;
    GLuint LightVbo;
    class OGLProgram* Program;
    class OGLTexture* StormTexture;
    class OGLProgram* LightProgram;
    glm::mat4 View;
    glm::mat4 Projection;
    std::vector<float> Vertices;
};