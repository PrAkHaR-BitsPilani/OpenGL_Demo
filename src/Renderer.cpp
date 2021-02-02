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