#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Ex09PostFxDraw 
{
public:
    Ex09PostFxDraw();
    ~Ex09PostFxDraw();
    void Update(float InDeltaTime);
private:
    GLuint SceneVao;
    GLuint SceneVbo;
    GLuint Ebo;
    class OGLProgram* SceneProgram;
    class OGLTexture* StormTexture;
    glm::mat4 View;
    glm::mat4 Projection;
    std::vector<float> SceneVertices;

     GLuint SceneFbo;
     GLuint SceneTexture;
     GLuint SceneRbo;
     class OGLProgram* FxProgram;
     GLuint FxVao;
     GLuint FxVbo;
};