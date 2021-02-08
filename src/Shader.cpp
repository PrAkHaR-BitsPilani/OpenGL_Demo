#include "Shader.h"
#include "Renderer.h"

#include <unordered_map>
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader(const std::string& filePath)
    : m_filepath(filePath) , m_RendererID(0)
{
    shaderSource source = parseShader(m_filepath);
    m_RendererID = createShader(source.VertexSource, source.FragementSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name , int value)
{
    GLCall(glUniform1i(getUniformLocation(name) , value));
}

void Shader::setUniform4f(const std::string& name , float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name) , v0 , v1 , v2 , v3));
}

int Shader::getUniformLocation(const std::string& name)
{
    if(m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_RendererID , name.c_str()));
    if(location == -1)
        std:cout << "[WARNING] uniform " << name << " not found" << endl;
    m_uniformLocationCache[name] = location;
    return location;
}

shaderSource Shader::parseShader(const string& filePath)
{
    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    ShaderType type = ShaderType::NONE;
    ifstream stream(filePath);
    string line;
    stringstream ss[2];
    while(getline(stream , line)){
        if(line.find("#shader") != string::npos){
            if(line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        }
        else{
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str() , ss[1].str()}; 
}

unsigned int Shader::compileShader(unsigned int type, const string &source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    
    if (result == GL_FALSE)
    {

        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)malloc(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        cout << "Error in " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader" << endl;
        cout << message << endl;
        GLCall(glDeleteShader(id));
        return -1;
    }

    return id;
}

unsigned int Shader::createShader(const string &vertexShader, const string &fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    assert(vs != -1);
    assert(fs != -1);

    GLCall(glAttachShader(program, fs));
    GLCall(glAttachShader(program, vs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    return program;
}