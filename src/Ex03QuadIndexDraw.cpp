#include "Ex03QuadIndexDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"

Ex03QuadIndexDraw::Ex03QuadIndexDraw() 
{
    Program = new OGLProgram("resources/shaders/triangleColor.vert", "resources/shaders/triangleColor.frag");
    
    std::vector<float> Vertices = {
            //Position             //Color
        -0.5f, -0.5f, 0.f,   0.f, 1.f, 0.f, 1.f,  // bottom-left
         0.5f, -0.5f, 0.f,   1.f, 0.f, 0.f, 1.f,  // bottom-right
         0.5f,  0.5f, 0.f,   0.f, 0.f, 1.f, 1.f,  // top-right  
        -0.5f,  0.5f, 0.f,   1.f, 0.f, 1.f, 1.f   // top-left
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2,  // Triangle Right
        0, 2, 3   // Triangle Left
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
    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE,  7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);
    glVertexAttribPointer(Location_1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indexes.size() * sizeof(uint32_t), Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();
}

Ex03QuadIndexDraw::~Ex03QuadIndexDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
}

void Ex03QuadIndexDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}