#pragma once
#include <string>
#include <unordered_map>

struct shaderSource{
    std::string VertexSource;
    std::string FragementSource;
};


class Shader
{
private:
    std::string m_filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string , int>m_uniformLocationCache;
    shaderSource parseShader(const std::string& filePath);
    unsigned int compileShader(unsigned int type, const std::string &source);
    unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);
    unsigned int getUniformLocation(const std::string& name);

public:
    Shader(const std::string& filePath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name , float v0, float v1, float v2, float v3);
};;