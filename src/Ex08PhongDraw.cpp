#include "Ex08PhongDraw.h"
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

Ex08PhongDraw::Ex08PhongDraw() 
{
    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    //6. Create Texture
    StormTexture = new OGLTexture("resources/models/stormtrooper.png");
    StormTexture->Bind(GL_TEXTURE0);

    //7. Enable Depth Testing
    glEnable(GL_DEPTH_TEST);

    //8. Enable Cull Face
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

    glm::vec3 PointLightPos = glm::vec3(4.f, 0, 0);
    Program->SetUniform("point_light_pos", PointLightPos);
}

Ex08PhongDraw::~Ex08PhongDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete StormTexture;
    delete Program;
}

void Ex08PhongDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    float Angle = 20.f * ElapsedTime;

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    /* Alternative preparing indipendent matrices
    glm::mat4 Identity =  glm::mat4(1.f);
    glm::mat4 Transl = glm::translate(Identity, glm::vec3(0, -4, 0));
    glm::mat4 Rotate = glm::rotate(Identity, glm::radians(Angle), glm::vec3(0, 1, 0));
    glm::mat4 Scale  = glm::scale(Identity, glm::vec3(2.f));
    glm::mat4 Model = Transl * Rotate * Scale;
    */

    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);

    Program->SetUniform("model", Model);

    glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
}