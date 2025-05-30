#pragma once
#include <glad/glad.h>

class Ex06QuadPerspDraw 
{
public:
    Ex06QuadPerspDraw();
    ~Ex06QuadPerspDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    class OGLProgram* Program;
    class OGLTexture* SmileTexture;
    class OGLTexture* BoxTexture;
};