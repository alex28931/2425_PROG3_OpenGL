#pragma once
#include <glad/glad.h>

class Ex04QuadColorDraw 
{
public:
    Ex04QuadColorDraw();
    ~Ex04QuadColorDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;

    class OGLProgram* Program;
};