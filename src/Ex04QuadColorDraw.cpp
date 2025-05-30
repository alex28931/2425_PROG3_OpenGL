#include "Ex04QuadColorDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>

Ex04QuadColorDraw::Ex04QuadColorDraw() 
{
    Program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");
    
    //Counter-clock wise. By now vertex data in NDC space.
    // |/ /|
    std::vector<float> Vertices = {
         //Triangle Right    //Colors
        -0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f,  // bottom-left
         0.5f, -0.5f, 0.f,   0.f, 1.f, 0.f,  // bottom-right
         0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f,  // top-right  

        //Triangle Left
        -0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f,  // bottom-left
         0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f,  // top-right  
        -0.5f,  0.5f, 0.f,   1.f, 1.f, 0.f   // top-left
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Crate VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);
    size_t DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    //GLint BaseColorLocation = glGetUniformLocation(Program->ProgramId, "base_color");
    Color BaseColor{0.5f, 0.5f, 0.5f, 1.f};
    //const GLfloat* BaseColorPtr = reinterpret_cast<GLfloat*>(&BaseColor);
    //glUniform4fv(BaseColorLocation, 1, BaseColorPtr);

    Program->SetUniform("base_color", BaseColor);
}

Ex04QuadColorDraw::~Ex04QuadColorDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
}

void Ex04QuadColorDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    Color TimedColor;
    TimedColor.R = sinf(ElapsedTime) * 0.5f + 0.5f; //[-1; 1] => [0; 1]
    TimedColor.G = cosf(ElapsedTime) * 0.5f + 0.5f;
    TimedColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimedColor.A = 1.f;

    //GLint BaseColorLocation = glGetUniformLocation(Program->ProgramId, "base_color");
    //glUniform4fv(BaseColorLocation, 1, reinterpret_cast<GLfloat*>(&TimedColor));
    Program->SetUniform("base_color", TimedColor);

    float scaleSpeed = 10.f;
    float scale = sinf(ElapsedTime * scaleSpeed) * 0.5f + 0.5f; // 0 - 100%
    //float scale = sinf(ElapsedTime * 0.1f);
    //float scale = -0.1f;
    
    //glUniform1f(glGetUniformLocation(Program->ProgramId, "scale"), scale);
    Program->SetUniform("scale", scale);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}