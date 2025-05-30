#include "Ex06QuadPerspDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"

Ex06QuadPerspDraw::Ex06QuadPerspDraw() 
{
    Program = new OGLProgram("resources/shaders/quadpersp.vert", "resources/shaders/quadpersp.frag");
    
    //Counter-clock wise. By now vertex data in NDC space.
    // |/ /|
    std::vector<float> Vertices = {
         //Triangle Right    //Uvs
        -0.5f, -0.5f, 0.f,   0.f, 0.f,  // bottom-left
         0.5f, -0.5f, 0.f,   1.f, 0.f,  // bottom-right
         0.5f,  0.5f, 0.f,   1.f, 1.f,  // top-right  

        //Triangle Left
        -0.5f, -0.5f, 0.f,   0.f, 0.f,  // bottom-left
         0.5f,  0.5f, 0.f,   1.f, 1.f,  // top-right  
        -0.5f,  0.5f, 0.f,   0.f, 1.f   // top-left
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    //6. Create Texture
    SmileTexture = new OGLTexture("resources/textures/smile.png");
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");

    SmileTexture->Bind(GL_TEXTURE0);

    //In case of using `uniform sampler2D smile_tex;` without "layout binding" directive
    //glUniform1i(glGetUniformLocation(Program->ProgramId, "smile_tex"), 0);

    BoxTexture->Bind(GL_TEXTURE1);

    //7. Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Ex06QuadPerspDraw::~Ex06QuadPerspDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete SmileTexture;
    delete BoxTexture;
    delete Program;
}

void Ex06QuadPerspDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}