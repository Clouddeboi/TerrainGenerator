#pragma once

#include <string>
#include <unordered_map>

/*
    Shader class

    Responsibilities:
    - Load shader source files (.vert / .frag)
    - Compile shaders
    - Link shader program
    - Bind/unbind program
    - Set uniforms (float, int, vectors, matrices)
*/

class Shader
{
public:
    //Constructor, loads and builds shader from file paths
    Shader(const std::string& vertexPath,
        const std::string& fragmentPath);

    //Destructor, cleans up OpenGL shader program
    ~Shader();

    //Activate/deactivate shader program
    void Bind() const;
    void Unbind() const;

    //Get raw OpenGL program ID
    unsigned int GetID() const { return m_RendererID; }
    
    //Uniform setters
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);

    void SetVec3(const std::string& name, float x, float y, float z);
    void SetVec4(const std::string& name, float x, float y, float z, float w);

    //4x4 matrix (used for 3D transforms)
    void SetMat4(const std::string& name, const float* mat);

private:
    
    //Reads a file from disk into a string, Used for shader source loading
    std::string LoadFile(const std::string& path);

    //Compiles a single shader (vertex or fragment), Returns OpenGL shader ID
    unsigned int CompileShader(unsigned int type,
        const std::string& source);

    //Gets uniform location with caching, we cache because glGetUniformLocation is slow if called every frame.
    int GetUniformLocation(const std::string& name);

private:
    //OpenGL shader program ID
    unsigned int m_RendererID = 0;

    //Cache uniform name to location
    std::unordered_map<std::string, int> m_UniformCache;
};