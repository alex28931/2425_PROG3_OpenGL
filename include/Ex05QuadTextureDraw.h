#pragma once
#include <glad/glad.h>

class Ex05QuadTextureDraw 
{
public:
    Ex05QuadTextureDraw();
    ~Ex05QuadTextureDraw();
    void Update(float InDeltaTime);
private:
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    class OGLProgram* Program;
    GLuint SmileTextureId;
    GLuint BoxTextureId;
};