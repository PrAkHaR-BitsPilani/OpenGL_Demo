#include "Renderer.h"
#include <iostream>

using namespace std;

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function , const char* fileName , int lineNo)
{
    GLenum error = glGetError();
    if(error){
        cout << "[OpenGL ERROR] " << error << "\nFUNCTION: " << function << "\nFILE: " << fileName << "\nLINE: " << lineNo <<  "\n"; 
        return false;
    }
    return true;
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va , const IndexBuffer& ib , const Shader& sh) const
{
    va.bind();
    ib.bind();
    sh.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount() , GL_UNSIGNED_INT , nullptr));
}