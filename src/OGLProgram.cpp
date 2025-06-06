#include "OGLProgram.h"

#include <vector>
#include <fstream>
#include <iostream>

static std::string ReadFile(const std::string& InPath);
static GLuint CreateShader(const std::string& InPath, GLuint InShaderType);
static GLuint CreateProgram(GLuint VertexShaderId, GLuint FragmentShaderId);


OGLProgram::OGLProgram(const std::string& InVertexPath, const std::string& InFragmPath)
{
    GLuint VertexShaderId = CreateShader(InVertexPath, GL_VERTEX_SHADER);
    GLuint FragmeShaderId = CreateShader(InFragmPath, GL_FRAGMENT_SHADER);

    ProgramId = CreateProgram(VertexShaderId, FragmeShaderId);
}

OGLProgram::~OGLProgram()
{
    glDeleteProgram(ProgramId);
}

void OGLProgram::Bind() {
    glUseProgram(ProgramId);
}

void OGLProgram::SetUniform(const std::string& InName, float InValue)
{
    glUniform1f(glGetUniformLocation(ProgramId, InName.c_str()), InValue);
}

void OGLProgram::SetUniform(const std::string& InName, const Color& InValue)
{
    glUniform4fv( glGetUniformLocation(ProgramId, InName.c_str()), 
        1, reinterpret_cast<const GLfloat*>(&InValue));
}

void OGLProgram::SetUniform(const std::string& InName, const glm::mat4& InValue)
{
    glUniformMatrix4fv(glGetUniformLocation(ProgramId, InName.c_str()), 1, GL_FALSE, &InValue[0][0]);
}

void OGLProgram::SetUniform(const std::string& InName, const glm::vec3& InValue)
{
    glUniform3fv(
        glGetUniformLocation(ProgramId, InName.c_str()),
        1, &InValue[0]
    );
}

std::string ReadFile(const std::string& InPath)
{
    std::ifstream InputStream(InPath, std::ios::ate);
    size_t FileSize = InputStream.tellg(); //cursor position in bytes

    std::string Text;
    Text.resize(FileSize);

    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&Text[0], FileSize);

    InputStream.close();
    return Text;
}

GLuint CreateShader(const std::string& InPath, GLuint InShaderType)
{
    std::string Text = ReadFile(InPath);
    const char* ShaderSource = Text.c_str();

    GLuint ShaderId = glCreateShader(InShaderType);
    glShaderSource(ShaderId, 1, &ShaderSource, NULL);
    glCompileShader(ShaderId);

    GLint Success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Success);
    if (!Success)  // 0 = false, 1 = true
    {
        GLint MaxLogLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetShaderInfoLog(ShaderId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        std::cout << "[ERRO] Shader Compilation failure: " << LogStr;
        throw std::runtime_error(LogStr);
    }
    return ShaderId;
}

GLuint CreateProgram(GLuint VertexShaderId, GLuint FragmentShaderId)
{
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);

    GLint Success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        GLint MaxLogLength;
        glGetShaderiv(ProgramId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetProgramInfoLog(ProgramId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        std::cout << "[ERRO] Program Linking failure: " << LogStr;
        throw std::runtime_error(LogStr);
    }

    glDeleteShader(VertexShaderId);
    glDeleteShader(FragmentShaderId);
    return ProgramId;
}
