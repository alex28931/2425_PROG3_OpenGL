#include "Ex10LightDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>
#include "OGLTexture.h"
#include "ObjParser.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Ex10LightDraw::Ex10LightDraw() 
{
    Program = new OGLProgram("resources/shaders/light_scene.vert", "resources/shaders/light_scene.frag");

    Obj obj;
    ObjParser::TryParseObj("resources/models/stormtrooper.obj", obj);

    for(size_t i = 0; i < obj.triangles.size(); ++i)
    {
        auto& t = obj.triangles[i];

        Vertices.push_back(t.v1.point.x);
        Vertices.push_back(t.v1.point.y);
        Vertices.push_back(t.v1.point.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.point.x);
        Vertices.push_back(t.v2.point.y);
        Vertices.push_back(t.v2.point.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.point.x);
        Vertices.push_back(t.v3.point.y);
        Vertices.push_back(t.v3.point.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }


    // Create Model VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    // Crate Model VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);
    size_t DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    // Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    // Create Texture
    StormTexture = new OGLTexture("resources/models/stormtrooper.png");


    // Setup light cube
    LightProgram = new OGLProgram("resources/shaders/light_light.vert", "resources/shaders/light_light.frag");
    
    std::vector<float> LightVertices = {
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

    glGenVertexArrays(1, &LightVao);
    glBindVertexArray(LightVao);

    glGenBuffers(1, &LightVbo);
    glBindBuffer(GL_ARRAY_BUFFER, LightVbo);
    glBufferData(GL_ARRAY_BUFFER, LightVertices.size() * sizeof(float), LightVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //GL States
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    //Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);
}

Ex10LightDraw::~Ex10LightDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteVertexArrays(1, &LightVao);
    glDeleteBuffers(1, &LightVbo);
    delete Program;
    delete LightProgram;
    delete StormTexture;
}

void Ex10LightDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0.f;
    ElapsedTime += InDeltaTime;

    float Angle = 20.f * ElapsedTime;

    glm::mat4 LightModel = glm::mat4(1.f);
    LightModel = glm::rotate(LightModel, glm::radians(Angle), glm::vec3(0, 1, 0));
    LightModel = glm::translate(LightModel, glm::vec3(4, 2, 0));

    glm::vec3 LightPos = glm::vec3(LightModel * glm::vec4(0, 0, 0, 1));

    // --- RENDER MODEL ---
    Program->Bind();

    Program->SetUniform("camera_pos", glm::vec3(0,0,8)); 
    Program->SetUniform("point_light_pos", LightPos);

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;

    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);

    StormTexture->Bind(GL_TEXTURE0);

    glBindVertexArray(Vao);
    glDrawArrays(GL_TRIANGLES, 0, Vertices.size() / 8);

    // --- RENDER LIGHT CUBE ---
    LightProgram->Bind();

    glm::mat4 LightCubeModel = glm::scale(LightModel, glm::vec3(0.2f));
    glm::mat4 LightMvp = Projection * View * LightCubeModel; 

    LightProgram->SetUniform("mvp", LightMvp);
    LightProgram->SetUniform("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(LightVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);  
}