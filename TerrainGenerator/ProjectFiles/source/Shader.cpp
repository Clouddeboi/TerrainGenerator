//OpenGL types are defined by GLAD
#include <glad/glad.h>

#include "../headers/Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

/*
    Constructor
    - Loads shader files
    - Compiles vertex + fragment shaders
    - Links them into a program
*/
Shader::Shader(const std::string& vertexPath,
    const std::string& fragmentPath)
{
    //Read shader files into strings
    std::string vertexSource = LoadFile(vertexPath);
    std::string fragmentSource = LoadFile(fragmentPath);

    //Compile shaders individually
    unsigned int vertexShader =
        CompileShader(GL_VERTEX_SHADER, vertexSource);

    unsigned int fragmentShader =
        CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    //Create program and attach shaders
    m_RendererID = glCreateProgram();

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    //Link program (combine shaders into executable GPU program)
    glLinkProgram(m_RendererID);

    //Check link errors
    int success;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[1024];

        glGetProgramInfoLog(m_RendererID, 1024, nullptr, infoLog);

        std::cout << "SHADER LINK ERROR:\n"
            << infoLog << std::endl;
    }

    //Shaders are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

//Destructor, Deletes GPU shader program
Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

//Bind shader (make it active)
void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

//Unbind shader (optional, mostly for debugging)
void Shader::Unbind() const
{
    glUseProgram(0);
}

//Reads a file from disk
std::string Shader::LoadFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "FAILED TO OPEN SHADER FILE: "
            << path << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}

//Compiles a shader (vertex or fragment)
unsigned int Shader::CompileShader(unsigned int type,
    const std::string& source)
{
    unsigned int shader = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    //Check compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[1024];

        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

        std::cout << "SHADER COMPILE ERROR:\n"
            << infoLog << std::endl;
    }

    return shader;
}

//Uniform location cache, Prevents repeated slow glGetUniformLocation calls
int Shader::GetUniformLocation(const std::string& name)
{
    //Check cache first
    if (m_UniformCache.find(name) != m_UniformCache.end())
        return m_UniformCache[name];

    //Query OpenGL
    int location =
        glGetUniformLocation(m_RendererID, name.c_str());

    //Store in cache
    m_UniformCache[name] = location;

    return location;
}

//Uniform setters
void Shader::SetBool(const std::string& name, bool value)
{
    glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec3(const std::string& name,
    float x, float y, float z)
{
    glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetVec4(const std::string& name,
    float x, float y, float z, float w)
{
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

//4x4 matrix uniform
void Shader::SetMat4(const std::string& name,
    const float* mat)
{
    glUniformMatrix4fv(
        GetUniformLocation(name),
        1,
        GL_FALSE,
        mat);
}