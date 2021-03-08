#pragma once
#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __builtin_trap()

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x , __FILE__ , __LINE__))

void GLClearError();

bool GLLogCall(const char* function , const char* fileName , int lineNo);

class Renderer
{
    public:
        void clear() const;
        void draw(const VertexArray& va , const IndexBuffer& ib , const Shader& sh) const;
        void drawPoints(const VertexArray& va , const Shader& sh , int count)const;
};