#include "Ex09PostFxDraw.h"
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

Ex09PostFxDraw::Ex09PostFxDraw() 
{
    SceneProgram = new OGLProgram("resources/shaders/postfx_scene.vert", "resources/shaders/postfx_scene.frag");

    Obj obj;
    ObjParser::TryParseObj("resources/models/stormtrooper.obj", obj);

    for(size_t i = 0; i < obj.triangles.size(); ++i)
    {
        auto& t = obj.triangles[i];

        SceneVertices.push_back(t.v1.point.x);
        SceneVertices.push_back(t.v1.point.y);
        SceneVertices.push_back(t.v1.point.z);
        SceneVertices.push_back(t.v1.uv.x);
        SceneVertices.push_back(t.v1.uv.y);
        SceneVertices.push_back(t.v1.normal.x);
        SceneVertices.push_back(t.v1.normal.y);
        SceneVertices.push_back(t.v1.normal.z);

        SceneVertices.push_back(t.v2.point.x);
        SceneVertices.push_back(t.v2.point.y);
        SceneVertices.push_back(t.v2.point.z);
        SceneVertices.push_back(t.v2.uv.x);
        SceneVertices.push_back(t.v2.uv.y);
        SceneVertices.push_back(t.v2.normal.x);
        SceneVertices.push_back(t.v2.normal.y);
        SceneVertices.push_back(t.v2.normal.z);

        SceneVertices.push_back(t.v3.point.x);
        SceneVertices.push_back(t.v3.point.y);
        SceneVertices.push_back(t.v3.point.z);
        SceneVertices.push_back(t.v3.uv.x);
        SceneVertices.push_back(t.v3.uv.y);
        SceneVertices.push_back(t.v3.normal.x);
        SceneVertices.push_back(t.v3.normal.y);
        SceneVertices.push_back(t.v3.normal.z);
    }


    //1. Create VAO
    glGenVertexArrays(1, &SceneVao);
    glBindVertexArray(SceneVao);

    //2. Crate VBO to load data
    glGenBuffers(1, &SceneVbo);
    glBindBuffer(GL_ARRAY_BUFFER, SceneVbo);
    size_t DataSize = SceneVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, SceneVertices.data(), GL_STATIC_DRAW);

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
    //SceneProgram->Bind();

    //6. Create Texture
    StormTexture = new OGLTexture("resources/models/stormtrooper.png");
    //StormTexture = new OGLTexture("resources/textures/wood-box.jpg");
    //StormTexture->Bind(GL_TEXTURE0);

    //7. Enable Depth Testing
    //glEnable(GL_DEPTH_TEST);

    //8. Enable Cull Face
    //glEnable(GL_CULL_FACE);

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
    SceneProgram->Bind();
    SceneProgram->SetUniform("point_light_pos", PointLightPos);

    // PostFX: Prepare Framebuffer for drawing "off-screen"
    glGenFramebuffers(1, &SceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);

    //1. Attach Color to framebuffer
    glGenTextures(1, &SceneTexture);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneTexture, 0);

    //2. Attach Depth to framebuffer
    glGenRenderbuffers(1, &SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, SceneRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind render buffer

    bool RboSuccess = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!RboSuccess) {
        std::cout << "Error creating framebuffer!\n";
        std::runtime_error("Error creating framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and back to Swapchain framebuffer


    // PREPARING FX PIPELINE
    FxProgram = new OGLProgram("resources/shaders/postfx_effect.vert", "resources/shaders/postfx_effect.frag");

    std::vector<float> QuadVertices = {
         //Triangle Right    //Uvs
        -1.f, -1.f, 0.f,   0.f, 0.f,  // bottom-left
         1.f, -1.f, 0.f,   1.f, 0.f,  // bottom-right
         1.f,  1.f, 0.f,   1.f, 1.f,  // top-right  

        //Triangle Left
        -1.f, -1.f, 0.f,   0.f, 0.f,  // bottom-left
         1.f,  1.f, 0.f,   1.f, 1.f,  // top-right    
        -1.f,  1.f, 0.f,   0.f, 1.f   // top-left
    };

    //1. Create VAO
    glGenVertexArrays(1, &FxVao);
    glBindVertexArray(FxVao);

    //2. Crate VBO to load data
    glGenBuffers(1, &FxVbo);
    glBindBuffer(GL_ARRAY_BUFFER, FxVbo);
    size_t QuadDataSize = QuadVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, QuadDataSize, QuadVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint QuadLocation_0 = 0;
    glVertexAttribPointer(QuadLocation_0, 3, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(QuadLocation_0);

    GLuint QuadLocation_1 = 1;
    glVertexAttribPointer(QuadLocation_1, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(QuadLocation_1);

}

Ex09PostFxDraw::~Ex09PostFxDraw() 
{
    glDeleteVertexArrays(1, &SceneVao);
    glDeleteBuffers(1, &SceneVbo);
    delete StormTexture;
    delete SceneProgram;
}

void Ex09PostFxDraw::Update(float InDeltaTime)
{
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    //1. Draw Scene (Pass 1)
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(SceneVao);
    SceneProgram->Bind();
    StormTexture->Bind(GL_TEXTURE0);


    float Angle = 20.f * ElapsedTime;

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;
    SceneProgram->SetUniform("mvp", Mvp);
    SceneProgram->SetUniform("model", Model);

    glDrawArrays(GL_TRIANGLES, 0, SceneVertices.size());


    //return;

    //2. Post FX (Pass 2)
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Back to Swapchain buffer
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(FxVao);
    FxProgram->Bind();
    FxProgram->SetUniform("time", ElapsedTime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}