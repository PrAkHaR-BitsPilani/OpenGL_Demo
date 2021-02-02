#pragma once
#include <glad/glad.h>

#define ASSERT(x) if(!(x)) __builtin_trap()

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x , __FILE__ , __LINE__))

void GLClearError();

bool GLLogCall(const char* function , const char* fileName , int lineNo);