#include "Ex07CubePerspDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"

Ex07CubePerspDraw::Ex07CubePerspDraw() 
{
    Program = new OGLProgram("resources/shaders/cubepersp.vert", "resources/shaders/cubepersp.frag");
    
    //Counter-clock wise. By now vertex data in NDC space.
    // |/ /|
    std::vector<float> Vertices = {
        //Position     //Uvs
        //FRONT FACE
        -1, -1,  1,     0, 0,  // bottom-left
         1, -1,  1,     1, 0,  // bottom-right
         1,  1,  1,     1, 1,  // top-right  
        -1,  1,  1,     0, 1,  // top-left
        -1, -1,  1,     0, 0,  // bottom-left
         1,  1,  1,     1, 1,  // top-right  
        
        // BACK FACE
         1, -1, -1,     0, 0,  // bottom-left
        -1, -1, -1,     1, 0,  // bottom-right
        -1,  1, -1,     1, 1,  // top-right  
         1,  1, -1,     0, 1,  // top-left
         1, -1, -1,     0, 0,  // bottom-left
        -1,  1, -1,     1, 1,  // top-right  
 
        //LEFT FACE
        -1, -1,-1,      0, 0,    //bottom-left
        -1, -1, 1,      1, 0,    //bottom-right
        -1,  1, 1,      1, 1,    //top-right
        -1,  1,-1,      0, 1,    //top-left 
        -1, -1,-1,      0, 0,    //bottom-left
        -1,  1, 1,      1, 1,    //top-right

        //RIGHT FACE      
         1, -1, 1,      0, 0,    //bottom-left
         1, -1,-1,      1, 0,    //bottom-right
         1,  1,-1,      1, 1,    //top-right
         1,  1, 1,      0, 1,    //top-left 
         1, -1, 1,      0, 0,    //bottom-left
         1,  1,-1,      1, 1,    //top-right

         //TOP FACE      
         -1, 1, 1,      0, 0,    //bottom-left
          1, 1, 1,      1, 0,    //bottom-right
          1, 1,-1,      1, 1,    //top-right
         -1, 1,-1,      0, 1,    //top-left 
         -1, 1, 1,      0, 0,    //bottom-left
          1, 1,-1,      1, 1,    //top-right
         
         //BOTTOM FACE
         -1,-1,-1,      0, 0,    //bottom-left
          1,-1,-1,      1, 0,    //bottom-right
          1,-1, 1,      1, 1,    //top-right
         -1,-1, 1,      0, 1,    //top-left 
         -1,-1,-1,      0, 0,    //bottom-left
          1,-1, 1,      1, 1,    //top-right
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
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");
    BoxTexture->Bind(GL_TEXTURE0);

    //7. Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    //8. Enable Cull Face
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK) //default
    //glFrontFace(GL_CCW) //default
}

Ex07CubePerspDraw::~Ex07CubePerspDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete BoxTexture;
    delete Program;
}

void Ex07CubePerspDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    Program->SetUniform("angle", 20.f * ElapsedTime);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}