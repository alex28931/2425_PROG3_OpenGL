#pragma once
#include <glad/glad.h>

class Ex07CubePerspDraw 
{
public:
    Ex07CubePerspDraw();
    ~Ex07CubePerspDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    class OGLProgram* Program;
    class OGLTexture* BoxTexture;
};