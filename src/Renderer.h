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

/// A renderer to make draw calls.
class Renderer
{
    public:
        ///Clears the color-buffer bit and the depth-buffer bit
        void clear() const;
        ///Renders triangles.
        ///
        ///Binds \p va , \p ib and \p sh and then makes a draw call.
        ///@param va reference to a VertexArray object
        ///@param ib reference to a IndexBuffer object
        ///@param sh reference to a Shader object
        ///@attention All the indexes present in \p ib are rendered.
        ///@warning A VertexBuffer object must be bound to \p va 
        ///@warning All the uniforms must be set in \p sh
        ///@see IndexBuffer VertexBuffer Shader
        void drawTriangles(const VertexArray& va , const IndexBuffer& ib , const Shader& sh) const;
        ///Renders points
        ///
        ///Binds \p va and \p sh and then makes a draw call.
        ///@param va reference to a VertexArray object
        ///@param sh reference to a Shader object
        ///@param count number of points to be rendered
        ///@warning A VertexBuffer object must be bound to \p va 
        ///@warning All the uniforms must be set in \p sh
        ///@see VertexBuffer Shader
        void drawPoints(const VertexArray& va , const Shader& sh , int count)const;
};