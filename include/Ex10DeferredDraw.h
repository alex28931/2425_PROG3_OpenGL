#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Ex10DeferredDraw
{
public:
    Ex10DeferredDraw();
    ~Ex10DeferredDraw();
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

     GLuint GFbo;
     GLuint DiffuseTexture;
     GLuint NormalTexture;
     GLuint PositionTexture;
     GLuint SceneRbo;
     class OGLProgram* BlendingProgram;
     GLuint BlendingVao;
     GLuint BlendingVbo;
};