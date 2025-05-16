#pragma once
#include <glad/glad.h>

class Ex03QuadIndexDraw 
{
public:
    Ex03QuadIndexDraw();
    ~Ex03QuadIndexDraw();
    void Update(float InDeltaTime);
private:
    GLuint ProgramId;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;

    class OGLProgram* Program;
};