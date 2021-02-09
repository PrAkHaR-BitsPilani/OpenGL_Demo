#pragma once
#include <string>
#include <unordered_map>

#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"

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
    int getUniformLocation(const std::string& name);

public:
    Shader(const std::string& filePath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform1i(const std::string& name , int value); 
    void setUniform4f(const std::string& name , float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name , const glm::mat4& mat); 
};;