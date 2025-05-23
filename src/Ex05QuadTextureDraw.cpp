#include "Ex05QuadTextureDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "OGLProgram.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Color {
    float R;
    float G;
    float B;
    float A;
};

GLuint CreateTexture(const std::string& InFilePath)
{
    stbi_set_flip_vertically_on_load(true);

    int Width, Height, Channels;
    unsigned char* Data = stbi_load(InFilePath.c_str(), &Width, &Height, &Channels, 0);
    if (Data == NULL) {
        std::cout << "Error Reading Image: " << InFilePath;
        throw std::runtime_error("Error Reading Image");
    }

    GLenum Format = Channels == 3 ? GL_RGB : GL_RGBA;

    GLuint TextureId;
    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    //Upload data to Gpu
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);

    //Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //MipMapping (Optional)
    glGenerateMipmap(GL_TEXTURE_2D);

    return TextureId;
}


Ex05QuadTextureDraw::Ex05QuadTextureDraw() 
{
    Program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");
    
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
    SmileTextureId = CreateTexture("resources/textures/smile.png");
    BoxTextureId = CreateTexture("resources/textures/wood-box.jpg");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SmileTextureId);

    //In case of using `uniform sampler2D smile_tex;` without "layout binding" directive
    //glUniform1i(glGetUniformLocation(Program->ProgramId, "smile_tex"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, BoxTextureId);

    //7. Enable Alpha Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Ex05QuadTextureDraw::~Ex05QuadTextureDraw() 
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteTextures(1, &SmileTextureId);
    glDeleteTextures(1, &BoxTextureId);
    delete Program;
}

void Ex05QuadTextureDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}